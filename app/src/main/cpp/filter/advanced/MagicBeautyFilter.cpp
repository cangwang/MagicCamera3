#include "MagicBeautyFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicBeautyFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/**
 * cangwang 2018.12.1
 */
MagicBeautyFilter::MagicBeautyFilter(){

}

MagicBeautyFilter::MagicBeautyFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"beauty.glsl")){

}

MagicBeautyFilter::~MagicBeautyFilter() {

}

void MagicBeautyFilter::onDestroy() {

}

void MagicBeautyFilter::onDrawArraysPre() {
    setBeautyLevel(5);
    GLfloat arrayValue[] = {mChangeWidth,mChangeHeight};
    glUniform2fv(mSingleStepOffsetLocation,1,arrayValue);
}

void MagicBeautyFilter::onDrawArraysAfter() {

}


void MagicBeautyFilter::onInit() {
    GPUImageFilter::onInit();
    mSingleStepOffsetLocation = glGetUniformLocation(mGLProgId,"singleStepOffset");
    mParamsLocation = glGetUniformLocation(mGLProgId,"params");
}

void MagicBeautyFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}

void MagicBeautyFilter::onInputSizeChanged(const int width, const int height){
    setTexelSize(width,height);
}

void MagicBeautyFilter::setTexelSize(const float w, const float h){
    mChangeWidth = 2.0f/w;
    mChangeHeight = 2.0f/h;

//    ALOGV("setTexlSize width =%f,height=%f",w,h);
}

void  MagicBeautyFilter::setBeautyLevel(int level){
    switch (level){
        case 1:
            glUniform1f(mParamsLocation,1.0f);
            break;
        case 2:
            glUniform1f(mParamsLocation,0.8f);
            break;
        case 3:
            glUniform1f(mParamsLocation,0.6f);
            break;
        case 4:
            glUniform1f(mParamsLocation,0.4f);
            break;
        case 5:
            glUniform1f(mParamsLocation,0.33f);
            break;
        default:
            break;
    }
}

