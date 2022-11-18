//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_PANO_BUILDER_H
#define GYROPANORAMA_PANO_BUILDER_H

#include "quaternion.h"

class PanoBuilder {
private:
public:
    PanoBuilder();

    void addImage(Mat &img, Quaternion &rot);
};

#endif //GYROPANORAMA_PANO_BUILDER_H
