#include "MagicXproIIFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicXproIIFilter"
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
MagicXproIIFilter::MagicXproIIFilter(){

}

MagicXproIIFilter::MagicXproIIFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"xproii_filter_shader.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicXproIIFilter::~MagicXproIIFilter() {

}

void MagicXproIIFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicXproIIFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], i+3);
        }
    }
}

void MagicXproIIFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}


void MagicXproIIFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
    }
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicXproIIFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"xpromap.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"vignettemap_new.png");
}