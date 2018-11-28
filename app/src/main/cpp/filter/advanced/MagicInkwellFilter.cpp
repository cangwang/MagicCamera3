#include "MagicInkwellFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicInkwellFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cool滤镜
 */

MagicInkwellFilter::MagicInkwellFilter(){

}

MagicInkwellFilter::MagicInkwellFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"inwell.glsl")),inputTextureHandles(0){

}

MagicInkwellFilter::~MagicInkwellFilter() {

}

void MagicInkwellFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicInkwellFilter::onDrawArraysPre() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicInkwellFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
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
    glUniform1f(mGLStrengthLocation, 1.0f);
    inputTextureHandles = loadTextureFromAssets(mAssetManager,"hudsonbackground.png");
}