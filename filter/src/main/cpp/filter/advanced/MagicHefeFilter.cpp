#include "MagicHefeFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicHefeFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.1
 */
MagicHefeFilter::MagicHefeFilter(){

}

MagicHefeFilter::MagicHefeFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"hefe.glsl")){
    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicHefeFilter::~MagicHefeFilter() {

}

void MagicHefeFilter::onDestroy() {
    glDeleteTextures(len,inputTextureHandles);
    *inputTextureHandles={0};
}

void MagicHefeFilter::onDrawArraysPre() {
//    ALOGV("onDrawArraysAfter len",len);
    glUniform1f(mGLStrengthLocation, 1.0f);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glUniform1i(inputTextureUniformLocations[i], (i+3));
        }
    }
}

void MagicHefeFilter::onDrawArraysAfter() {
//    ALOGV("onDrawArraysAfter len",len);
    for (int i = 0; i < len; ++i) {
        if (inputTextureHandles[i] != 0) {
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
            glActiveTexture(GL_TEXTURE0);
            ALOGV("onDrawArraysAfter inputTextureHandles[%d]",i);
        }
    }
}


void MagicHefeFilter::onInit() {
    GPUImageFilter::onInit();
    for (int i = 0; i < len; ++i) {
        std::ostringstream ss;
        ss << "inputImageTexture" << 2+i;
        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,
                                                               ss.str().c_str());
    }
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicHefeFilter::onInitialized() {
    GPUImageFilter::onInitialized();
//    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"rise_mask1.jpg");
//    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"hefemap.png");
//    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"rise_mask2.jpg");
//    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"edgeburn.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"hefemap.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"hefemetal.png");
    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
}