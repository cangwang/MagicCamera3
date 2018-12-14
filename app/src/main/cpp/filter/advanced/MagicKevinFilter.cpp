#include "MagicKevinFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicKevinFilter"
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
MagicKevinFilter::MagicKevinFilter(){

}

MagicKevinFilter::MagicKevinFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"kevin_new.glsl")){

}

MagicKevinFilter::~MagicKevinFilter() {

}

void MagicKevinFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicKevinFilter::onDrawArraysPre() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicKevinFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicKevinFilter::onInit() {
    GPUImageFilter::onInit();
    inputTextureUniformLocations = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicKevinFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 1.0f);
    inputTextureHandles = loadTextureFromAssets(mAssetManager,"kelvinmap.png");
}