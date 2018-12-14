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
    mGLTextureBuffer = getRotation(ROTATION_90, true, false);
}

MagicHefeFilter::~MagicHefeFilter() {

}

void MagicHefeFilter::onDestroy() {
//    glDeleteTextures(len,inputTextureHandles);
//    *inputTextureHandles={0};
    glDeleteTextures(1,&inputTextureHandles0);
    glDeleteTextures(1,&inputTextureHandles1);
    glDeleteTextures(1,&inputTextureHandles2);
    glDeleteTextures(1,&inputTextureHandles3);
    inputTextureHandles0 =0;
    inputTextureHandles1 =0;
    inputTextureHandles2 =0;
    inputTextureHandles3 =0;
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

    if (inputTextureHandles0 != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles0);
        glUniform1i(inputTextureUniformLocations0, 3);
    }

    if (inputTextureHandles1 != 0) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles1);
        glUniform1i(inputTextureUniformLocations1, 4);
    }

    if (inputTextureHandles2 != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles2);
        glUniform1i(inputTextureUniformLocations2, 5);
    }

    if (inputTextureHandles3 != 0) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles3);
        glUniform1i(inputTextureUniformLocations3, 6);
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

    if (inputTextureHandles0 != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles0);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles1 != 0) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles1);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles2 != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles2);
        glActiveTexture(GL_TEXTURE0);
    }

    if (inputTextureHandles3 != 0) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles3);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicHefeFilter::onInit() {
    GPUImageFilter::onInit();
//    for (int i = 0; i < len; ++i) {
//        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
//    }
    inputTextureUniformLocations0 = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    inputTextureUniformLocations1 = glGetUniformLocation(mGLProgId,"inputImageTexture3");
    inputTextureUniformLocations2 = glGetUniformLocation(mGLProgId,"inputImageTexture4");
    inputTextureUniformLocations3 = glGetUniformLocation(mGLProgId,"inputImageTexture5");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicHefeFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mGLStrengthLocation, 0.0f);
//    inputTextureHandles[0] = loadTextureFromAssets(mAssetManager,"rise_mask1.jpg");
//    inputTextureHandles[1] = loadTextureFromAssets(mAssetManager,"hefemap.png");
//    inputTextureHandles[2] = loadTextureFromAssets(mAssetManager,"rise_mask2.jpg");
//    inputTextureHandles[3] = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
    inputTextureHandles0 = loadTextureFromAssets(mAssetManager,"edgeburn.png");
    inputTextureHandles1 = loadTextureFromAssets(mAssetManager,"hefemap.png");
    inputTextureHandles2 = loadTextureFromAssets(mAssetManager,"hefemetal.png");
    inputTextureHandles3 = loadTextureFromAssets(mAssetManager,"hefesoftlight.png");
}