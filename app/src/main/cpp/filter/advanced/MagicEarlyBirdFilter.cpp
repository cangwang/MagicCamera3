#include "MagicEarlyBirdFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicEarlyBirdFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cool滤镜
 */

MagicEarlyBirdFilter::MagicEarlyBirdFilter(){

}

MagicEarlyBirdFilter::MagicEarlyBirdFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"brannan.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicEarlyBirdFilter::~MagicEarlyBirdFilter() {

}

void MagicEarlyBirdFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={-0};
}

void MagicEarlyBirdFilter::onDrawArraysPre() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], 3);
        }
    }
}

void MagicEarlyBirdFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicEarlyBirdFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
        mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    }
}

void MagicEarlyBirdFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 1.0f);
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"earlybirdcurves.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"earlybirdoverlaymap_new.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"vignettemap_new.png");
    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"earlybirdlowout.png");
    inputTextureHandles[4] = loadTextureFromAssets(mAssetManager,"earlybirdmap.png");

}