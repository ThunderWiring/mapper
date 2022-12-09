//
// Created by Bassam on 12/27/2021.
//

#include "unityPluginInterface.h"
#include <chrono>

static inline long getCurrentTimestamp() {
    return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    ).count();
}

UnityPluginInterface::UnityPluginInterface() : last_ts(getCurrentTimestamp()) {
}

void UnityPluginInterface::getPanorama(
        RenderAPI *renderer, void *texture, int width, int height) {
    Mat out;
    joinImageThreads();
    if (out.empty()) {
        LOGD("UnityPluginInterface", "empty panorama image");
        return;
    } else if (out.channels() == 3) {
        cv::cvtColor(out, out, cv::COLOR_BGR2RGBA);
    }

    renderer->EndModifyTexture(texture, width, height, 4, out.data);
}

void UnityPluginInterface::getImageFromUnity(UnityColor32 *bytes, int width, int height) {
    LOGE("UnityPluginInterface", "getImageFromUnity - unimplemented function");
}

void UnityPluginInterface::getImageFromUnity(
        Quaternion q, UnityColor32 *pixels, int width, int height) {
    long curr_ts = getCurrentTimestamp();
    if (curr_ts - last_ts < MIN_FRAME_INTERVAL_MILLISEC) {
        return;
    }
    last_ts = curr_ts;

    Mat img;
    getMatFromPixels(pixels, width, height, img);
    if (img.empty()) {
        LOGE("UnityPluginInterface", "function %s - passed image is empty", __FUNCTION__);
        return;
    } else if (img.channels() == 4) {
        cv::cvtColor(img, img, cv::COLOR_RGBA2BGR);
    }

    image_buffer.push_back(std::make_shared<Mat>(img));
    rotation_buffer.push_back(std::make_shared<Quaternion>(q));
    image_workers.emplace_back([&]() {
        pano_builder.addImage(*image_buffer.back(), *rotation_buffer.back());
    });
}

void UnityPluginInterface::getPanoramaDims(int &width, int &height) {
    LOGD("getPanoramaDims", "total images to process %d", image_buffer.size());
    pano_builder.getPanoDims(width, height);
}

void UnityPluginInterface::joinImageThreads() {
    for (auto &img_worker : image_workers) {
        if (img_worker.joinable()) {
            img_worker.join();
        }
    }
}

