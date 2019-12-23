#include "MagicWaldenFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicWaldenFilter"
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
MagicWaldenFilter::MagicWaldenFilter(){

}

MagicWaldenFilter::MagicWaldenFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"walden.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicWaldenFilter::~MagicWaldenFilter() {

}

void MagicWaldenFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicWaldenFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glUniform1i(inputTextureUniformLocations[i], i+3);
//        }
//    }
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
}

void MagicWaldenFilter::onDrawArraysAfter() {
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glActiveTexture(GL_TEXTURE0);
//        }
//    }
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
}


void MagicWaldenFilter::onInit() {
    GPUImageFilter::onInit();
//    for (int i = 0; i < len; ++i) {
//        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
//    }
    inputTextureUniformLocations[0] = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    inputTextureUniformLocations[1] = glGetUniformLocation(mGLProgId,"inputImageTexture3");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicWaldenFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"walden_map.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"vignette_map.png");
}