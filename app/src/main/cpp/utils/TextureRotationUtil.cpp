#include "TextureRotationUtil.h"

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

//获取角度
static float* TextureRotationUtil::getRotation(const RotationUtil::Rotation rotation, const bool flipHorizontal, const bool flipVertical){
    const float* rotateTex;
    switch (rotation){
        case RotationUtil::ROTATION_90:
            rotateTex = TEXTURE_ROTATED_90;
            break;
        case RotationUtil::ROTATION_180:
            rotateTex = TEXTURE_ROTATED_180;
            break;
        case RotationUtil::ROTATION_270:
            rotateTex = TEXTURE_ROTATED_270;
            break;
        case RotationUtil::NORMAL:
        default:
            rotateTex = TEXTUTRE_NO_ROTATION;
            break;
    }
    if (flipHorizontal){
        float flipTran[]={
                flip(rotateTex[0]),rotateTex[1],
                flip(rotateTex[2]),rotateTex[3],
                flip(rotateTex[4]),rotateTex[5],
                flip(rotateTex[6]),rotateTex[7]
        };
        rotateTex = flipTran;
    }

    if (flipVertical){
        float flipTran[]={
                rotateTex[0],flip(rotateTex[1]),
                rotateTex[2],flip(rotateTex[3]),
                rotateTex[4],flip(rotateTex[5]),
                rotateTex[6],flip(rotateTex[7])
        };
        rotateTex = flipTran;
    }

    return const_cast<float *>(rotateTex);
}

float TextureRotationUtil::flip(const float i) {
    if (i == 0.0f){
        return 1.0f;
    }
    return 0.0f;
}