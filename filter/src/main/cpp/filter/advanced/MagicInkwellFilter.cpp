#include "MagicInkwellFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicInkwellFilter"
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
MagicInkwellFilter::MagicInkwellFilter(){

}

MagicInkwellFilter::MagicInkwellFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"inkwell.glsl")),inputTextureHandles(0){

}

MagicInkwellFilter::~MagicInkwellFilter() {

}

void MagicInkwellFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicInkwellFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    if (inputTextureHandles != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicInkwellFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicInkwellFilter::onInit() {
    GPUImageFilter::onInit();
    inputTextureUniformLocations = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicInkwellFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    inputTextureHandles = loadTextureFromAssets(mAssetManager,"hudsonbackground.png");
}