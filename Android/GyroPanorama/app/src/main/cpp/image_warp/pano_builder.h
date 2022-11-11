//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_PANO_BUILDER_H
#define GYROPANORAMA_PANO_BUILDER_H

#include "quaternion.h"

class PanoBuilder {
    struct ImageBlock {
        Mat img, H, warped;
        Quaternion rot;
        cv::Point2f com; // center-of-mass
    };

    struct ImageCorners {
//        ImageCorners(): p1(cv::Point2d(0,0)), p2(cv::Point2d(0,0)), p3(cv::Point2d(0,0)), p4(cv::Point2d(0,0))
//        {}
        cv::Point2f p1, p2, p3, p4;

        inline void print(const char *tag) const {
            LOGD(tag, "corners:\n\tp1:(%f, %f), \n\tp2:(%f, %f), \n\tp3:(%f, %f), \n\tp4:(%f, %f)\n\t",
                 p1.x, p1.y,
                 p2.x, p2.y,
                 p3.x, p3.y,
                 p4.x, p4.y
            );
        }
    };

private:
    Mat pano;

    void get_warped_image_corners(Mat &img, Mat &H, ImageCorners &corners_out) const;

    /**
     * Adds the warpped frame into the panorama frame, s.t. the center of gravity
     * for the warpped, will align with the the uv coords that corresponds to the
     * rotation vector.
     * */
    void assign_to_pano(ImageBlock &img_block);

    /**
     * Returns the center of mass for the image after being warped with the transformation H
     * the center of mass for a rect is exactly at the point where the two diagonals meet
     * */
    cv::Point2i get_center_of_mass(Mat &orig_img, Mat &H);

    /**
     * Crops the warped image to fit within the pano frame after offset according to the uv coords.
     * @param dx, dy the offset on cols according to the uv correction
     * */
    void crop_img(Mat &cropped, int dx, int dy, ImageBlock &img_block) const;

public:
    PanoBuilder();

    void addImage(Mat &img, Quaternion &rot);

    void getPanorama(Mat &out);
};

#endif //GYROPANORAMA_PANO_BUILDER_H
