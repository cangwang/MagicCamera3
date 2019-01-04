#include "MagicShineWhiteFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicShineWhiteFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 放大
 */
MagicShineWhiteFilter::MagicShineWhiteFilter(){

}

MagicShineWhiteFilter::MagicShineWhiteFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"whiteshine_f.glsl")){

}

MagicShineWhiteFilter::~MagicShineWhiteFilter() {
}

void MagicShineWhiteFilter::onDestroy() {
}

void MagicShineWhiteFilter::onDrawArraysPre() {
    if (mFrames<=mMiddleFrames){
        mProgress = mFrames*1.0f/mMiddleFrames;
    } else{
        mProgress = 2.0f-mFrames*1.0f/mMiddleFrames;
    }
    mFrames++;
    if (mFrames>mMaxFrames){
        mFrames = 0;
    }

    glUniform1f(mAdditionColorLocation,mProgress);
}

void MagicShineWhiteFilter::onDrawArraysAfter() {

}


void MagicShineWhiteFilter::onInit() {
    GPUImageFilter::onInit();
    mAdditionColorLocation = glGetUniformLocation(mGLProgId,"uAdditionalColor");
}

void MagicShineWhiteFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}