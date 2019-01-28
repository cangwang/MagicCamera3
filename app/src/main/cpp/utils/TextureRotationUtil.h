#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "Rotation.h"

#ifndef _TextureRotationUtil
#define _TextureRotationUtil

static GLfloat CUBE[] = {
        -1.0f,-1.0f,
        1.0f,-1.0f,
        -1.0f,1.0f,
        1.0f,1.0f
};

float* getRotation(const Rotation rotation, const bool flipHorizontal, const bool flipVertical);
float* getRotation(int degree, const bool flipHorizontal, const bool flipVertical);
float flip(const float i);

#endif