//
// Created by Bassam on 7/3/2022.
//

#ifndef GYROPANORAMA_LOGGER_H
#define GYROPANORAMA_LOGGER_H

#include <stddef.h>
#include <android/log.h>

#define LOGD(tag, ...) (__android_log_print(ANDROID_LOG_DEBUG, (tag), __VA_ARGS__))
#define LOGE(tag, ...) (__android_log_print(ANDROID_LOG_ERROR, (tag), __VA_ARGS__))
#define STR_BOOL(b) (b) ? "true" : "false"

#define LOG_IMG(tag, img) LOGD(tag, "isEmpty: %s, size:[%d x %d], elementSize:%d", \
    STR_BOOL(img.empty()), img.rows, img.cols, img.elemSize())

#define LOG_EXP(tag, e) LOGE(tag, "Exception at %d:%s: %s", __LINE__, __FUNCTION__, e.what())

#endif //GYROPANORAMA_LOGGER_H
