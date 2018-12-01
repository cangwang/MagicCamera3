#include "MagicCrayonFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicCrayonFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cangwang 2018.12.1
 */
MagicCrayonFilter::MagicCrayonFilter(){

}

MagicCrayonFilter::MagicCrayonFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"default_vertex.glsl"), readShaderFromAsset(assetManager,"crayon.glsl")){

}

MagicCrayonFilter::~MagicCrayonFilter() {

}

void MagicCrayonFilter::onDestroy() {

}

void MagicCrayonFilter::onDrawArraysPre() {

}

void MagicCrayonFilter::onDrawArraysAfter() {

}


void MagicCrayonFilter::onInit() {
    GPUImageFilter::onInit();
    mSingleStepOffsetLocation = glGetUniformLocation(mGLProgId,"singleStepOffset");
    mStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
    glUniform1f(mStrengthLocation, 2.0f);
}

void MagicCrayonFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    glUniform1f(mStrengthLocation, 0.5f);
}

void MagicCrayonFilter::onInputSizeChanged(const int width, const int height){
    setTexelSize(width,height);
}

void MagicCrayonFilter::setTexelSize(const float w, const float h){
    float arrayValue[] = {1.0f/w,1.0f/h};
    glUniform2fv(mSingleStepOffsetLocation,1,arrayValue);
}
