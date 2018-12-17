#include "MagicToasterFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

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
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glUniform1i(inputTextureUniformLocations[i], i+3);
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
    if (inputTextureHandles[4] != 0) {
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[4]);
        glUniform1i(inputTextureUniformLocations[4], 7);
    }

}

void MagicToasterFilter::onDrawArraysAfter() {
//    for (int i = 0; i < len; ++i) {
//        if (inputTextureHandles[i] != 0) {
//            glActiveTexture(static_cast<GLenum>(GL_TEXTURE3 + i));
//            glBindTexture(GL_TEXTURE_2D, inputTextureHandles[i]);
//            glActiveTexture(GL_TEXTURE0);
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

    if (inputTextureHandles[4] != 0) {
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles[4]);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicToasterFilter::onInit() {
    GPUImageFilter::onInit();
//    for (int i = 0; i < len; ++i) {
//        inputTextureUniformLocations[i] = glGetUniformLocation(mGLProgId,"inputImageTexture"+(2+i));
//    }
    inputTextureUniformLocations[0] = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    inputTextureUniformLocations[1] = glGetUniformLocation(mGLProgId,"inputImageTexture3");
    inputTextureUniformLocations[2] = glGetUniformLocation(mGLProgId,"inputImageTexture4");
    inputTextureUniformLocations[3] = glGetUniformLocation(mGLProgId,"inputImageTexture5");
    inputTextureUniformLocations[4] = glGetUniformLocation(mGLProgId,"inputImageTexture5");
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