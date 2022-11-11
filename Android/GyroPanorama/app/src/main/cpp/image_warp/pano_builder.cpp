//
// Created by Bassam on 7/3/2022.
//

#include "pano_builder.h"

PanoBuilder::PanoBuilder() {
    pano = Mat(/*hight, width*/);
}

void PanoBuilder::addImage(Mat &img, Quaternion &rot) {
    LOG_IMG("pano_builder-addImage", img);
}

void PanoBuilder::assign_to_pano(ImageBlock &img_block) {
    // get UV coords
    UVcoord uv = img_block.rot.toUV();

    // do offset
    int dx = (int) (uv.u - img_block.com.x);
    int dy = (int) (uv.v - img_block.com.y);
    LOGD("pano_builder-assign", "dx=%d, dy=%d, uv=(%f, %f), CoM=(%f, %f)", dx, dy, uv.u, uv.v,
         img_block.com.x, img_block.com.y);

    // crop if necessary
    auto is_outside_bound = [&](cv::Point2f &p) {
        int new_x(p.x + dx), new_y(p.y + dy);
        bool is_valid_x = new_x >= 0 && new_x < pano.cols;
        bool is_valid_y = new_y >= 0 && new_y < pano.rows;
        return is_valid_x && is_valid_y;
    };
    ImageCorners warped_corners;
    get_warped_image_corners(img_block.img, img_block.H, warped_corners);
    bool is_img_out_of_bound = is_outside_bound(warped_corners.p1) ||
                               is_outside_bound(warped_corners.p2) ||
                               is_outside_bound(warped_corners.p3) ||
                               is_outside_bound(warped_corners.p4);
    Mat cropped = img_block.img;
    if (is_img_out_of_bound) {
        crop_img(cropped, dx, dy, img_block);
    }

    // assign to pano
    LOGD("pano_builder", "assign to pano - roi: Rect(%d, %d, %d, %d)", std::abs(dx), std::abs(dy),
         cropped.cols, cropped.rows);
    auto roi = cv::Rect(std::fabs(dx), std::fabs(dy), cropped.cols, cropped.rows);
    cropped.copyTo(pano(roi));
}

cv::Point2i PanoBuilder::get_center_of_mass(Mat &orig_img, Mat &H) {
    ImageCorners warped_corners;
    get_warped_image_corners(orig_img, H, warped_corners);
    warped_corners.print("pano_builder-corners");

    float p2[2] = {warped_corners.p2.x, warped_corners.p2.y};
    float p3[2] = {warped_corners.p3.x, warped_corners.p3.y};
    float p4[2] = {warped_corners.p4.x, warped_corners.p4.y};

    // find the diagonal intersection formed by the 4 points (the 4th point is (0,0) )
    // using determenants (Cramer's law) https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line

    float p = (p2[1] * (p3[0] - p4[0]) - p2[0] * (p3[1] - p4[1]));
    float p_x = p2[0] / p;
    float p_y = p2[1] / p;

    LOGD("pano_builder", "get CoM (%f, %f), p = %f", p_x, p_y, p);

    return {(int)p_x, (int)p_y};
}

void PanoBuilder::get_warped_image_corners(Mat &orig_img, Mat &H, ImageCorners &corners_out) const {

    LOG_IMG("pano_builder-corners orig_img", orig_img);
    LOGD("pano_builder-corners H", "\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f",
         H.at<float>(0, 0), H.at<float>(0, 1), H.at<float>(0, 2),
         H.at<float>(1, 0), H.at<float>(1, 1), H.at<float>(1, 2),
         H.at<float>(2, 0), H.at<float>(2, 1), H.at<float>(2, 2)
    );

    float p2[3] = {0.0, (float) orig_img.cols - 1, 1};
    float p3[3] = {(float) orig_img.rows - 1, 0.0, 1};
    float p4[3] = {(float) orig_img.rows - 1, (float) orig_img.cols - 1, 1};

    // p1_vec is the origin (0,0)
    Mat p2_vec = Mat(3, 1, CV_32F, p2);
    Mat p3_vec = Mat(3, 1, CV_32F, p3);
    Mat p4_vec = Mat(3, 1, CV_32F, p4);

    Mat p2_w = H * p2_vec;
    Mat p3_w = H * p3_vec;
    Mat p4_w = H * p4_vec;

    corners_out.p1.x = 0;
    corners_out.p1.y = 0;

    corners_out.p2.x = p2_w.at<float>(0, 0);
    corners_out.p2.y = p2_w.at<float>(1, 0);

    corners_out.p3.x = p3_w.at<float>(0, 0);
    corners_out.p3.y = p3_w.at<float>(1, 0);

    corners_out.p4.x = p4_w.at<float>(0, 0);
    corners_out.p4.y = p4_w.at<float>(1, 0);
}

void PanoBuilder::crop_img(Mat &cropped, int dx, int dy, ImageBlock &img_block) const {
    int x_overflow = pano.cols - (dx + img_block.warped.cols);
    int y_overflow = pano.rows - (dy + img_block.warped.rows);

    uint top_left_x(std::abs(dx)), top_left_y(std::abs(dy));

    if (x_overflow < 0) {
        top_left_x = std::max(0, dx);
    }
    if (y_overflow < 0) {
        top_left_y = std::max(0, dy);
    }

    auto mask = cv::Rect(
            top_left_x, top_left_y,
            img_block.warped.cols - top_left_y, img_block.warped.rows - top_left_x);
    img_block.warped(mask).copyTo(cropped);
}


