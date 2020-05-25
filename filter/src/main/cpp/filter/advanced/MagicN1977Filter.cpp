#include <sstream>
#include "MagicN1977Filter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicN1977Filter"
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
MagicN1977Filter::MagicN1977Filter(){

}

MagicN1977Filter::MagicN1977Filter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"n1977.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicN1977Filter::~MagicN1977Filter() {

}

void MagicN1977Filter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicN1977Filter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], (i+3));
        }
    }
}

void MagicN1977Filter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicN1977Filter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        std::ostringstream ss;
        ss << "inputImageTexture" << 2+i;
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,
                                                               ss.str().c_str());
    }
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicN1977Filter::onInitialized() {
    GPUImageFilter::onInitialized();

    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"n1977map.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"n1977blowout.png");
}