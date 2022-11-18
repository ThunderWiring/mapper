//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_IMAGE_READER_H
#define GYROPANORAMA_IMAGE_READER_H

#include "common_libs.h"

struct UnityColor32 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct UVcoord {
    float u, v;
};

void getMatFromPixels(UnityColor32 *bytes, int width, int height, cv::Mat &out);

#endif //GYROPANORAMA_IMAGE_READER_H
