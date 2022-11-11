#include <jni.h>
#include <string>
#include "common_libs.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_thunderwiring_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}