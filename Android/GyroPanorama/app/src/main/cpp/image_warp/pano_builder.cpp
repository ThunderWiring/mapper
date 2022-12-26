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
        pano_image = img.clone();
        return;
    }
    stitch_image(img);
}

void PanoBuilder::stitch_image(Mat &img) {
    Mat res;
    stitchImages(pano_image, img, res);
    pano_image = res;
}

void PanoBuilder::getPanoDims(int& w, int& h) const {
    if(pano_image.empty()){
        w = 0;
        h = 0;
        return;
    }
    Mat tmp =pano_image;
    remove_black_background(tmp);
    w = tmp.cols;
    h = tmp.rows;
}

void PanoBuilder::getPanorama(Mat& out) const {
    out = pano_image.clone();
    remove_black_background(out);
}

void PanoBuilder::remove_black_background(Mat& img) const {
    /**
     * reference: https://stackoverflow.com/a/10647177/3983756
     * */

    /**Blur the image to remove noise, threshold the image, then find contours.*/
    Mat modified_img = img;
    if (img.channels() == 3) {
        cv::cvtColor(img, modified_img, cv::COLOR_BGR2GRAY);
    }
    cv::medianBlur(modified_img, modified_img, 3);
    cv::threshold(modified_img, modified_img, 1, 255, 0);

    /** find largest contour */
    auto contours = vector<vector<cv::Point>>();
    cv::findContours(modified_img, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    vector<cv::Point> largest_contour;
    float max_area = -1;
    for (auto& contour : contours) {
        float area = cv::contourArea(contour);
        if (max_area < area) {
            max_area = area;
            largest_contour = contour;
        }
    }

    /** remove redundant points from the largest contour while preserving the corners */
    vector<cv::Point> approx_contour;
    double epsilon = 0.01 * cv::arcLength(largest_contour, true);
    cv::approxPolyDP(largest_contour, approx_contour, epsilon, true);

    /** find the corners
        3 of the 4 corners must be (0,0), (0, y), (x, 0)
        the 4th point must be the one with the largest product of both coords,
        since it's the furthest corner.
     */
    double max_coord_prod = 0;
    cv::Point p1(cv::Point(0,0)), p2, p3, p4;
    for (auto& point : approx_contour) {
        if (point.x == 0 && point.y != 0) {
            p3 = point;
        } else if (point.x != 0 && point.y == 0) {
            p2 = point;
        } else if (point.x * point.y > max_coord_prod) {
            p4 = point;
            max_coord_prod = point.x * point.y;
        }
    }
    p4.x = std::fmin(p4.x, p2.x);
    p4.y = std::fmin(p4.y, p3.y);
    auto frame = cv::Rect(p1, p4);

    img = img(frame);
}
