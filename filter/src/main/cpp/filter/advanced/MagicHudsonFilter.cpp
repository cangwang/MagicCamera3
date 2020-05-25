#include <sstream>
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
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], (i+3));
            ALOGV("onDrawArraysPre inputTextureUniformLocations[%d]",i);
        }
    }
    glUniform1f(mGLStrengthLocation, 1.0f);
}

void MagicHudsonFilter::onDrawArraysAfter() {
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
            ALOGV("onDrawArraysAfter inputTextureHandles[%d]",i);
        }
    }
}


void MagicHudsonFilter::onInit() {
    GPUImageFilter::onInit();
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    for (int i = 0; i < len; ++i) {
        std::ostringstream ss;
        ss << "inputImageTexture" << 2+i;
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,
                                                               ss.str().c_str());
    }
}

void MagicHudsonFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"hudsonbackground.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"overlaymap.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"hudsonmap.png");
}