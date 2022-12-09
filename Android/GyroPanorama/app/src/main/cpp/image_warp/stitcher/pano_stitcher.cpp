//
// Created by Bassam on 11/25/2022.
//

#include "pano_stitcher.h"
#include "opencv2/features2d.hpp"
#include <opencv2/calib3d.hpp>

#define MIN_GOOD_MATCHES_COUNT 10

using cv::ORB;
using cv::DescriptorMatcher;

void stitchImages(Mat &img1, Mat &img2, Mat &res) {
    vector<KeyPoint> kpts1, kpts2;
    Mat desc1, desc2;
    getImageKeypointsAndDescriptors(img1, kpts1, desc1);
    getImageKeypointsAndDescriptors(img2, kpts2, desc2);

    if (kpts1.empty() || kpts2.empty()) {
        LOGE("pano_stitcher", "could not extract keypoints from images");
        return;
    }

    vector<cv::DMatch> good_matches;
    matchImages(desc1, desc2, good_matches);
    if (good_matches.size() < MIN_GOOD_MATCHES_COUNT) {
        LOGE("pano_stitcher",
             "could not find enough good matches between images. Min required is %d, found %d",
             MIN_GOOD_MATCHES_COUNT, good_matches.size());
        return;
    }

    Mat H;
    getHomographyFromMatches(kpts1, kpts2, good_matches, H);
    warpImagesToPano(img1, img2, H, res);
}

void getImageKeypointsAndDescriptors(Mat &img, vector<cv::KeyPoint> &kpts, Mat &desc) {
    cv::Ptr<ORB> detector = ORB::create();
    detector->detectAndCompute(img, cv::noArray(), kpts, desc);
}

void matchImages(Mat &desc1, Mat &desc2, vector<cv::DMatch> &good_matches) {
    cv::Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(
            DescriptorMatcher::BRUTEFORCE_HAMMING);
    vector<vector<cv::DMatch> > knn_matches;
    matcher->knnMatch(desc1, desc2, knn_matches, 2);

    // Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    for (auto &knn_matche : knn_matches) {
        if (knn_matche[0].distance < ratio_thresh * knn_matche[1].distance) {
            good_matches.push_back(knn_matche[0]);
        }
    }
}

void getHomographyFromMatches(
        vector<KeyPoint> &kpts1,
        vector<KeyPoint> &kpts2,
        vector<cv::DMatch> &good_matches,
        Mat &H
) {
    // Localize the object
    vector<Point2f> obj;
    vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); i++) {
        // Get the keypoints from the good matches
        obj.push_back(kpts1[good_matches[i].queryIdx].pt);
        scene.push_back(kpts2[good_matches[i].trainIdx].pt);
    }
    H = cv::findHomography(obj, scene, cv::RANSAC);
}

void warpImagesToPano(Mat &img1, Mat &img2, Mat &H, Mat &res) {
    warpPerspective(img1, res, H, cv::Size(img1.cols + img2.cols, img1.rows));
    Mat half(res, cv::Rect(0, 0, img2.cols, img2.rows));
    img2.copyTo(half);
}