#include "MagicBrooklynFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicBrooklynFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.1
 */
MagicBrooklynFilter::MagicBrooklynFilter(){

}

MagicBrooklynFilter::MagicBrooklynFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"brooklyn.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicBrooklynFilter::~MagicBrooklynFilter() {

}

void MagicBrooklynFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicBrooklynFilter::onDrawArraysPre() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], 3);
        }
    }
}

void MagicBrooklynFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicBrooklynFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
        mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    }
}

void MagicBrooklynFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 1.0f);
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"brooklynCurves1.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"filter_map_first.jpg");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"brooklynCurves2.png");
}