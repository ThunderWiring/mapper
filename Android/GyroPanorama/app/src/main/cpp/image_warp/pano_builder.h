//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_PANO_BUILDER_H
#define GYROPANORAMA_PANO_BUILDER_H

#include "quaternion.h"
#include "opencv2/core.hpp"
#include "opencv2/stitching.hpp"

using cv::Stitcher;

class PanoBuilder {
private:
    Mat pano_image; // contains the whole panorama image
    Stitcher::Mode mode;

    /**
     * Stitch @param img with pano_image.
     * */
    void stitch_image(Mat& img);
public:
    PanoBuilder();

    void addImage(Mat &img, Quaternion &rot);
    int getPanoDims(int& w, int& h) const;
};

#endif //GYROPANORAMA_PANO_BUILDER_H
