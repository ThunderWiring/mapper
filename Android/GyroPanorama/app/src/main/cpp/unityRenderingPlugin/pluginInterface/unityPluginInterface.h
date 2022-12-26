//
// Created by Bassam on 12/27/2021.
//

#ifndef GYROPANORAMA_UNITYPLUGININTERFACE_H
#define GYROPANORAMA_UNITYPLUGININTERFACE_H

#include "image_reader.h"
#include "pano_builder.h"
#include "RenderAPI.h"


using namespace std::chrono;

constexpr int MIN_FRAME_INTERVAL_MILLISEC = 1000; // 1 sec

class UnityPluginInterface {
private:
    PanoBuilder pano_builder;

    long last_ts;
    vector <thread> image_workers;
    vector <std::shared_ptr<Mat>> image_buffer;
    vector <std::shared_ptr<Quaternion>> rotation_buffer;

    void joinImageThreads();

public:
    UnityPluginInterface();

    void getPanoramaDims(int &width, int &height);

    /**
     * Assigns the panorama image pixels to the texture via the pointer of the texture.
     * */
    void getPanorama(RenderAPI *renderer, void *texture, int width, int height);


    void getImageFromUnity(UnityColor32 *bytes, int width, int height);

    void getImageFromUnity(
            Quaternion q, UnityColor32 *bytes, int width, int height);

    void resetImages();
};

#endif //GYROPANORAMA_UNITYPLUGININTERFACE_H
