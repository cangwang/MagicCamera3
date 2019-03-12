#include "MagicGlitchFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicGlitchFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 毛刺
 */
MagicGlitchFilter::MagicGlitchFilter(){

}

MagicGlitchFilter::MagicGlitchFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"glitch_f.glsl")){

}

MagicGlitchFilter::~MagicGlitchFilter() {
    free(mDriftSequence);
    free(mJitterSequence);
    free(mThreshHoldSequence);
}

void MagicGlitchFilter::onDestroy() {

}

void MagicGlitchFilter::onDrawArraysPre() {
    glUniform2f(mScanLineJitterLocation,mJitterSequence[mFrames],mThreshHoldSequence[mFrames]);
    glUniform1f(mColorDriftLocation,mDriftSequence[mFrames]);
    mFrames ++;
    if (mFrames>mMaxFrames){
        mFrames = 0;
    }
}

void MagicGlitchFilter::onDrawArraysAfter() {

}


void MagicGlitchFilter::onInit() {
    GPUImageFilter::onInit();
    mScanLineJitterLocation = glGetUniformLocation(mGLProgId,"uScanLineJitter");
    mColorDriftLocation = glGetUniformLocation(mGLProgId,"uColorDrift");
}

void MagicGlitchFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    //颜色偏移量
    mDriftSequence = new float[9]{0.0f, 0.03f, 0.032f, 0.035f, 0.03f, 0.032f, 0.031f, 0.029f, 0.025f};
    //偏移的x值
    mJitterSequence = new float[9]{0.0f, 0.03f, 0.01f, 0.02f, 0.05f, 0.055f, 0.03f, 0.02f, 0.025f};
    //偏移的y值
    mThreshHoldSequence = new float[9]{1.0f, 0.965f, 0.9f, 0.9f, 0.9f, 0.6f, 0.8f, 0.5f, 0.5f};
}