//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_QUATERNION_H
#define GYROPANORAMA_QUATERNION_H

#include "image_reader.h"

static constexpr float PI = 3.14159265359;
static constexpr float PI2 = 6.28318530718; // 2 * PI

/**
 * Stores the rotation received from unity which is the quaternion 4d vector
 * as well as the eulerAngles 3d vector
 * */
struct RotationInfo {
    float w, xs, ys, zs;
    float euler_x, euler_y, euler_z;
};

/**
 * A quaternion is a 4 component vector representing a rotation of an object in 3D space.
 * q = (cos(theta/2), x*sin(theta/2), y*sin(theta/2), z*sin(theta/2))
 * */
class Quaternion {
private:
    float w, x, y, z;

    float euler_x, euler_y, euler_z;

    /**
     * the quaternion vector is a unit vector, but the actual sphere can be scaled.
     * */
    float scale;
    float theta;

public:
    Quaternion(RotationInfo &rot, float vec_scale = 1.0f) {
        euler_x = rot.euler_x;
        euler_y = rot.euler_y;
        euler_z = rot.euler_z;

        scale = vec_scale;
        theta = std::acosf(rot.w);
        float sin = std::sinf(theta);
        w = rot.w;
        x = rot.xs / sin;
        y = rot.ys / sin;
        z = rot.zs / sin;
        LOGD("Quaternion", "theta = %f, sin = %f, w=%f, x=%f, y=%f, z=%f", theta, sin, w, x, y, z);
        theta *= 2;
    }

    Quaternion(
            float w, float xs, float ys, float zs, float vec_scale = 1.0f) {
    }

    void get4x4Matrix(Mat &matrix) const;

    void get3x3Matrix(Mat &matrix) const;

    /**
     * Returns the uv coordinates that correspond to the (x,y,z) point on the
     * sphere where the quaternion vector points to.
     * */
    UVcoord toUV() const;

    inline void print(const char *tag) const {
        LOGD(tag, "Q(%f, %f, %f, %f)", w, x, y, z);
    }
};

#endif //GYROPANORAMA_QUATERNION_H
