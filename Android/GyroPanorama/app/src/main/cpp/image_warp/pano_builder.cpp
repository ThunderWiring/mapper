//
// Created by Bassam on 7/3/2022.
//

#include "pano_builder.h"


PanoBuilder::PanoBuilder():
    pano_image(Mat()),
    mode(Stitcher::PANORAMA) {
}

/**
 Approach1: regular stitching - find keypoints in each image and match them
 Approach2: depending on the angle of the image, take the relevant quarter of the current
            panorama to find matching keypoints. That way save ~50%-75% of time

 * */

void PanoBuilder::addImage(Mat &img, Quaternion &rot) {
    if (pano_image.empty()) {
        pano_image = Mat(img);
        return;
    }
    stitch_image(img);
}

void PanoBuilder::stitch_image(Mat &img) {
    cv::Ptr<Stitcher> stitcher = Stitcher::create(mode);

    // Command to stitch all the images present in the image array
    auto imgs = vector<Mat>({img, pano_image});
    Stitcher::Status status = stitcher->stitch(imgs, pano_image);

    if (status != Stitcher::OK) {
        // Check if images could not be stitched
        // status is OK if images are stitched successfully
        LOGE("PanoBuilder::stitch_image", "cannot stitch image with pano - status code %d", status);
    }
}


