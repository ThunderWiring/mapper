//
// Created by Bassam on 7/3/2022.
//

#include "quaternion.h"

void Quaternion::get4x4Matrix(Mat &matrix) const {
    float data[16] = {
            w, -x, -y, -1 * z,
            x, w, -1 * z, y,
            y, z, w, -1 * x,
            z, -1 * y, x, w
    };

    print("Quaternion");
    LOGD("Quaternion-matrix4x4",
         "%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n",
         w, -x, -y, -1 * z,
         x, w, -1 * z, y,
         y, z, w, -1 * x,
         z, -1 * y, x, w
    );
    matrix = Mat(4, 4, CV_32F, data);
}

void Quaternion::get3x3Matrix(Mat &matrix) const {
    if (matrix.empty() || matrix.cols != 3 || matrix.rows != 3) {
        LOGE("", "passed wrong mat dims");
        return;
    }
    LOGD("Quaternion-matrix4x4",
         "%f, %f, %f,\n%f, %f, %f,\n%f, %f, %f,\n%f, %f, %f,\n",
         w, -z, y,
         z, w, -x,
         -y, x, w
    );
    matrix.at<float>(0, 0) = w;
    matrix.at<float>(0, 1) = -z;
    matrix.at<float>(0, 2) = y;
    matrix.at<float>(1, 0) = z;
    matrix.at<float>(1, 1) = w;
    matrix.at<float>(1, 2) = -x;
    matrix.at<float>(2, 0) = -y;
    matrix.at<float>(2, 1) = x;
    matrix.at<float>(2, 2) = w;
}

UVcoord Quaternion::toUV() const {
    return {
            .u =  0.5f + std::atan2f(z, x) / (PI2),
            .v =  0.5f + std::asinf(y) / PI,
    };
}