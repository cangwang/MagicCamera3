#include "MagicPixarFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicPixarFilter"
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
MagicPixarFilter::MagicPixarFilter(){

}

MagicPixarFilter::MagicPixarFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"pixar.glsl")){

}

MagicPixarFilter::~MagicPixarFilter() {

}

void MagicPixarFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicPixarFilter::onDrawArraysPre() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicPixarFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicPixarFilter::onInit() {
    GPUImageFilter::onInit();
    inputTextureUniformLocations = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicPixarFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 1.0f);
    inputTextureHandles = loadTextureFromAssets(mAssetManager,"pixar_curves.png");
}