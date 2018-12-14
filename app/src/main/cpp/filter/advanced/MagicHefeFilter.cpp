#include "MagicHefeFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

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
//    GET_ARRAY_LEN(inputTextureHandles,len);
}

MagicHefeFilter::~MagicHefeFilter() {

}

void MagicHefeFilter::onDestroy() {
//    glDeleteTextures(len,inputTextureHandles);
//    *inputTextureHandles={0};
    glDeleteTextures(1,&inputTextureHandles[0]);
    glDeleteTextures(1,&inputTextureHandles[1]);
    glDeleteTextures(1,&inputTextureHandles[2]);
    glDeleteTextures(1,&inputTextureHandles[3]);
    inputTextureHandles[0] =0;
    inputTextureHandles[1] =0;
    inputTextureHandles[2] =0;
    inputTextureHandles[3] =0;
}

void MagicHefeFilter::onDrawArraysPre() {
//    ALOGV("onDrawArraysAfter len",len);
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glUniform1i(inputTextureUniformLocations[i], (i+3));
//            ALOGV("onDrawArraysPre inputTextureUniformLocations[%d]",i);
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

    if (inputTextureHandles[2] != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[2]);
        glUniform1i(inputTextureUniformLocations[2], 5);
    }

    if (inputTextureHandles[3] != 0) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[3]);
        glUniform1i(inputTextureUniformLocations[3], 6);
    }
}

void MagicHefeFilter::onDrawArraysAfter() {
//    ALOGV("onDrawArraysAfter len",len);
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glActiveTexture(GL_TEXTURE0);
//            ALOGV("onDrawArraysAfter inputTextureHandles[%d]",i);
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

    if (inputTextureHandles[2] != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[2]);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles[3] != 0) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[3]);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicHefeFilter::onInit() {
    GPUImageFilter::onInit();
//    for (int i = 0; i < len; ++i) {
//        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
//    }
    inputTextureUniformLocations[0] = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    inputTextureUniformLocations[1]= glGetUniformLocation(mGLProgId,"inputImageTexture3");
    inputTextureUniformLocations[2] = glGetUniformLocation(mGLProgId,"inputImageTexture4");
    inputTextureUniformLocations[3] = glGetUniformLocation(mGLProgId,"inputImageTexture5");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicHefeFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 0.0f);
//    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"rise_mask1.jpg");
//    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"hefemap.png");
//    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"rise_mask2.jpg");
//    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"edgeburn.png");
    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"hefemap.png");
    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"hefemetal.png");
    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
}