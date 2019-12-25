#include "MagicLookupFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicLookupFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.17
 */
MagicLookupFilter::MagicLookupFilter(){

}

MagicLookupFilter::MagicLookupFilter(AAssetManager *assetManager,std::string table)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"lookup.glsl")),table(table){

}

MagicLookupFilter::~MagicLookupFilter() {

}

void MagicLookupFilter::onDestroy() {
    glDeleteTextures(1,&inputTextureHandles);
}

void MagicLookupFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 1.0f);
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glUniform1i(inputTextureUniformLocations,3);
    }
}

void MagicLookupFilter::onDrawArraysAfter() {
    if (inputTextureHandles != 0) {
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE3));
        glBindTexture(GL_TEXTURE_2D, inputTextureHandles);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicLookupFilter::onInit() {
    GPUImageFilter::onInit();
    inputTextureUniformLocations = glGetUniformLocation(mGLProgId,"inputImageTexture2");
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicLookupFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    inputTextureHandles = loadTextureFromAssets(mAssetManager,table.c_str());
}