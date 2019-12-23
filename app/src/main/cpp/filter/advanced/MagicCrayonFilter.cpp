#include "MagicCrayonFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicCrayonFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/**
 * cangwang 2018.12.1
 */
MagicCrayonFilter::MagicCrayonFilter(){

}

MagicCrayonFilter::MagicCrayonFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"crayon.glsl")){

}

MagicCrayonFilter::~MagicCrayonFilter() {

}

void MagicCrayonFilter::onDestroy() {

}

void MagicCrayonFilter::onDrawArraysPre() {
    glUniform1f(mStrengthLocation, 0.5f);
    GLfloat arrayValue[] = {mChangeWidth,mChangeHeight};
    glUniform2fv(mSingleStepOffsetLocation,1,arrayValue);
}

void MagicCrayonFilter::onDrawArraysAfter() {

}


void MagicCrayonFilter::onInit() {
    GPUImageFilter::onInit();
    mSingleStepOffsetLocation = glGetUniformLocation(mGLProgId,"singleStepOffset");
    mStrengthLocation = glGetUniformLocation(mGLProgId,"strength");
}

void MagicCrayonFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}

void MagicCrayonFilter::onInputSizeChanged(const int width, const int height){
    setTexelSize(width,height);
}

void MagicCrayonFilter::setTexelSize(const float w, const float h){
    mChangeWidth = 1.0f/w;
    mChangeHeight = 1.0f/h;

//    ALOGV("setTexlSize width =%f,height=%f",w,h);
}

