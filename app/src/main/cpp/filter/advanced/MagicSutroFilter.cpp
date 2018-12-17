#include "MagicSutroFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicSutroFilter"
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
MagicSutroFilter::MagicSutroFilter(){

}

MagicSutroFilter::MagicSutroFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"sutro.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicSutroFilter::~MagicSutroFilter() {

}

void MagicSutroFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicSutroFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], i+3);
        }
    }
}

void MagicSutroFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicSutroFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
    }
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicSutroFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"vignette_map.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"sutrometal.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"softlight.png");
    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"sutroedgeburn.png");
    inputTextureHandles[4] = loadTextureFromAssets(mAssetManager,"sutrocurves.png");

}