//
// Created by Bassam on 11/25/2022.
//

#ifndef GYROPANORAMA_PANO_STITCHER_H
#define GYROPANORAMA_PANO_STITCHER_H

#include "common_libs.h"

using cv::KeyPoint;

void stitchImages(Mat &img1, Mat &img2, Mat& res);

void getImageKeypointsAndDescriptors(
        Mat &img, vector<cv::KeyPoint> &kpts, Mat &desc);

void matchImages(Mat &desc1, Mat &desc2, vector<cv::DMatch> &good_matches);

void getHomographyFromMatches(
        vector<KeyPoint> &kpts1,
        vector<KeyPoint> &kpts2,
        vector<cv::DMatch> &good_matches,
        Mat &H
);

void warpImagesToPano(Mat& img1, Mat& img2, Mat& H, Mat& res);

#endif //GYROPANORAMA_PANO_STITCHER_H
