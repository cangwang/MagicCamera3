#include "MagicNashvilleFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicNashvilleFilter"
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
MagicNashvilleFilter::MagicNashvilleFilter(){

}

MagicNashvilleFilter::MagicNashvilleFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"nashville.glsl")){

}

MagicNashvilleFilter::~MagicNashvilleFilter() {

}

void MagicNashvilleFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicNashvilleFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    if (inputTextureHandles != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicNashvilleFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicNashvilleFilter::onInit() {
    GPUImageFilter::onInit();
    inputTextureUniformLocations = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicNashvilleFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    inputTextureHandles = loadTextureFromAssets(mAssetManager,"nashvillemap.png");
}