#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/bitmap/BitmapOperation.h>
#include "src/main/cpp/filter/gpuimage/gpuimagefilter.h"

#define LOG_TAG "Rotation"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


class RotationUtil{
public:
    static enum Rotation {
        NORMAL, ROTATION_90, ROTATION_180, ROTATION_270
    };

    static int asInt(Rotation rotation);
    static Rotation fromInt(int roation);
};


