#include "MagicSketchFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicSketchFilter"
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
MagicSketchFilter::MagicSketchFilter(){

}

MagicSketchFilter::MagicSketchFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"sketch.glsl")){
}

MagicSketchFilter::~MagicSketchFilter() {

}

void MagicSketchFilter::onDestroy() {

}

void MagicSketchFilter::onDrawArraysPre() {
    glUniform1f(mGLStrengthLocation, 0.5f);
    float size[]={mChangeWidth,mChangeHeight};
    glUniform2fv(mSingleStepOffsetLocation,1,size);
}

void MagicSketchFilter::onDrawArraysAfter() {

}


void MagicSketchFilter::onInit() {
    GPUImageFilter::onInit();
    mGLStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    mSingleStepOffsetLocation = glGetUniformLocation(mGLProgId,"singleStepOffset");
}

void MagicSketchFilter::onInitialized() {
    GPUImageFilter::onInitialized();
}

void MagicSketchFilter::onInputSizeChanged(const int width, const int height) {
    mChangeWidth = 1.0f/width;
    mChangeHeight = 1.0f/height;

}