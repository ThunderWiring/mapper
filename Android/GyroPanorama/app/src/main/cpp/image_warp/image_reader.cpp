//
// Created by Bassam on 7/3/2022.
//

#include "image_reader.h"

void getMatFromPixels(UnityColor32 *pixels, int width, int height, cv::Mat &out) {
    size_t pixel_count(width * height), pixel_size(sizeof(UnityColor32));
    auto bytes = new unsigned char[pixel_count * pixel_size];

    for (size_t i(0), byte_idx(0); i < pixel_count; i++, byte_idx += pixel_size) {
        bytes[byte_idx] = pixels[i].r;
        bytes[byte_idx + 1] = pixels[i].g;
        bytes[byte_idx + 2] = pixels[i].b;
        bytes[byte_idx + 3] = pixels[i].a;
    }
    out = Mat(height, width, CV_8UC4, bytes);
}