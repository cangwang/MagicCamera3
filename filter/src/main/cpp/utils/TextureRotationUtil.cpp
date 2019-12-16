#include "TextureRotationUtil.h"

#define LOG_TAG "TextureRotationUtil"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


GLfloat TEXTURE_NO_ROTATION[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
};

GLfloat TEXTURE_ROTATED_90[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
};

GLfloat TEXTURE_ROTATED_180[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
};
GLfloat TEXTURE_ROTATED_270[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
};

GLfloat CUBE[] = {
        -1.0f,-1.0f,
        1.0f,-1.0f,
        -1.0f,1.0f,
        1.0f,1.0f
};

//获取角度
float* getRotation(const Rotation rotation, const bool flipHorizontal, const bool flipVertical){
    const float* rotateTex;
    //调整角度
    switch (rotation){
        case ROTATION_90:
            rotateTex = TEXTURE_ROTATED_90;
            break;
        case ROTATION_180:
            rotateTex = TEXTURE_ROTATED_180;
            break;
        case ROTATION_270:
            rotateTex = TEXTURE_ROTATED_270;
            break;
        case NORMAL:
        default:
            rotateTex = TEXTURE_NO_ROTATION;
            break;
    }
    //垂直翻转
    if (flipHorizontal){
        const static float flipTran[]={
                flip(rotateTex[0]),rotateTex[1],
                flip(rotateTex[2]),rotateTex[3],
                flip(rotateTex[4]),rotateTex[5],
                flip(rotateTex[6]),rotateTex[7]
        };
        return const_cast<float *>(flipTran);
    }

    //水平翻转
    if (flipVertical){
        const static float flipTran[]={
                rotateTex[0],flip(rotateTex[1]),
                rotateTex[2],flip(rotateTex[3]),
                rotateTex[4],flip(rotateTex[5]),
                rotateTex[6],flip(rotateTex[7])
        };
        return const_cast<float *>(flipTran);
    }

    return const_cast<float *>(rotateTex);
}

//获取角度
float* getRotation(int degree, const bool flipHorizontal, const bool flipVertical){
    const float* rotateTex;
    //调整角度
    switch (degree){
        case 90:
            rotateTex = TEXTURE_ROTATED_90;
            break;
        case 180:
            rotateTex = TEXTURE_ROTATED_180;
            break;
        case 270:
            rotateTex = TEXTURE_ROTATED_270;
            break;
        case 0:
        default:
            rotateTex = TEXTURE_NO_ROTATION;
            break;
    }
    //垂直翻转
    if (flipHorizontal){
        const static float flipTran[]={
                flip(rotateTex[0]),rotateTex[1],
                flip(rotateTex[2]),rotateTex[3],
                flip(rotateTex[4]),rotateTex[5],
                flip(rotateTex[6]),rotateTex[7]
        };
        return const_cast<float *>(flipTran);
    }

    //水平翻转
    if (flipVertical){
        const static float flipTran[]={
                rotateTex[0],flip(rotateTex[1]),
                rotateTex[2],flip(rotateTex[3]),
                rotateTex[4],flip(rotateTex[5]),
                rotateTex[6],flip(rotateTex[7])
        };
        return const_cast<float *>(flipTran);
    }

    return const_cast<float *>(rotateTex);
}

float flip(const float i) {
    if (i == 0.0f){
        return 1.0f;
    }
    return 0.0f;
}

float* getCube(){
    return CUBE;
}