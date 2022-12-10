//
// Created by Bassam on 7/3/2022.
//

#include "pano_builder.h"
#include "pano_stitcher.h"

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
    Mat res;
    stitchImages(img, pano_image, res);
    pano_image = res;
}

void PanoBuilder::getPanoDims(int& w, int& h) const {
    if(pano_image.empty()){
        w = 0;
        h = 0;
        return;
    }
    w = pano_image.cols;
    h = pano_image.rows;
}

void PanoBuilder::getPanorama(Mat& out) const {
    out = pano_image.clone();
}
