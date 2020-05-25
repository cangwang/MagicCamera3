#include <sstream>
#include "MagicToasterFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicToasterFilter"
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
MagicToasterFilter::MagicToasterFilter(){

}

MagicToasterFilter::MagicToasterFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"toaster2_filter_shader.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicToasterFilter::~MagicToasterFilter() {

}

void MagicToasterFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicToasterFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], i+3);
        }
    }
}

void MagicToasterFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicToasterFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        std::ostringstream ss;
        ss << "inputImageTexture" << 2+i;
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,
                                                               ss.str().c_str());
    }
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicToasterFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"toastermetal.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"toastersoftlight.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"toastercurves.png");
    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"toasteroverlaymapwarm.png");
    inputTextureHandles[4] = loadTextureFromAssets(mAssetManager,"toastercolorshift.png");

}