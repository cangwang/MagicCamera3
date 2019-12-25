#include "MagicHudsonFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicHudsonFilter"
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
MagicHudsonFilter::MagicHudsonFilter(){

}

MagicHudsonFilter::MagicHudsonFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"hudson.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicHudsonFilter::~MagicHudsonFilter() {

}

void MagicHudsonFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles[0]);
    glDeleteTextures(1,&inputTextureHandles[1]);
    glDeleteTextures(1,&inputTextureHandles[2]);
    inputTextureHandles[0] =0;
    inputTextureHandles[1] =0;
    inputTextureHandles[2] =0;
}

void MagicHudsonFilter::onDrawArraysPre() {
    if (inputTextureHandles[0] != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[0]);
        glUniform1i(inputTextureUniformLocations[0], 3);
    }

    if (inputTextureHandles[1] != 0) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[1]);
        glUniform1i(inputTextureUniformLocations[1], 4);
    }

    if (inputTextureHandles[2] != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[2]);
        glUniform1i(inputTextureUniformLocations[2], 5);
    }
    glUniform1f(mGLStrengthLocation, 1.0f);
}

void MagicHudsonFilter::onDrawArraysAfter() {
    if (inputTextureHandles[0] != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[0]);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles[1] != 0) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[1]);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles[2] != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[2]);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicHudsonFilter::onInit() {
    GPUImageFilter::onInit();
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    inputTextureUniformLocations[0] = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    inputTextureUniformLocations[1] = glGetUniformLocation(mGLProgId,"inputImageTexture3");
    inputTextureUniformLocations[2] = glGetUniformLocation(mGLProgId,"inputImageTexture4");
}

void MagicHudsonFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"hudsonbackground.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"overlaymap.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"hudsonmap.png");
}