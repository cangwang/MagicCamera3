#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "Rotation.h"

#ifndef _TextureRotationUtil
#define _TextureRotationUtil


float* getRotation(const Rotation rotation, const bool flipHorizontal, const bool flipVertical);
float* getRotation(int degree, const bool flipHorizontal, const bool flipVertical);
float flip(const float i);
float* getCube();
float* getNoneMatrix();
float* getFullCoords();
float* getFullTexture();

#endif