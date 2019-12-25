#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "Rotation.h"

#define LOG_TAG "Rotation"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

int RotationUtil::asInt(Rotation rotation) {
    switch (rotation){
        case NORMAL:
            return 0;
        case ROTATION_90:
            return 90;
        case ROTATION_180:
            return 180;
        case ROTATION_270:
            return 270;
        default:
            ALOGE("Unknown Rotation!");
            return 0;
    }
}

Rotation RotationUtil::fromInt(int roation) {
    switch(roation){
        case 0:
            return NORMAL;
        case 90:
            return ROTATION_90;
        case 180:
            return ROTATION_180;
        case 270:
            return ROTATION_270;
        default:
            ALOGE("roation is an unknown rotation. Needs to be either 0, 90, 180 or 270!");
            return NORMAL;
    }
}
