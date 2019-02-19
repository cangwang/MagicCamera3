#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "Rotation.h"

#ifndef _TextureRotationUtil
#define _TextureRotationUtil

const static GLfloat NONE_MATRIX[] = {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f,
};

const static GLfloat FULL_RECTANGLE_COORDS[] = {
        -1.0f, -1.0f,   // 0 bottom left
        1.0f, -1.0f,   // 1 bottom right
        -1.0f, 1.0f,   // 2 top left
        1.0f, 1.0f,   // 3 top right
};
const static GLfloat FULL_RECTANGLE_TEX_COORDS[] = {
        0.0f, 0.0f,     // 0 bottom left
        1.0f, 0.0f,     // 1 bottom right
        0.0f, 1.0f,     // 2 top left
        1.0f, 1.0f      // 3 top right
};


float* getRotation(const Rotation rotation, const bool flipHorizontal, const bool flipVertical);
float* getRotation(int degree, const bool flipHorizontal, const bool flipVertical);
float flip(const float i);
float* getCube();

#endif