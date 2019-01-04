#include "MagicSoulOutFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicSoulOutFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.1
 */
MagicSoulOutFilter::MagicSoulOutFilter(){

}

MagicSoulOutFilter::MagicSoulOutFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"soulout_v.glsl"), readShaderFromAsset(assetManager,"soulout_f.glsl")),mMvpMatrix(new float[16]){

}

MagicSoulOutFilter::~MagicSoulOutFilter() {
    free(mMvpMatrix);
}

void MagicSoulOutFilter::onDestroy() {
}

void MagicSoulOutFilter::onDrawArraysPre() {
    mProgress = (float)mFrames/mMaxFrames;
    if (mProgress>1){
        mProgress = 0;
    }
    mFrames++;
    if (mFrames>mMaxFrames + mSkipFrames){
        mFrames = 0;
    }
    setIdentityM(mMvpMatrix,0);
    glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
    float backAlpha = 1;
    if (mProgress >0){
        alpha = static_cast<float>(0.2 - mProgress * 0.2);
        backAlpha = 1-alpha;
    }

    glUniform1f(mAlphaLocation,backAlpha);

}

void MagicSoulOutFilter::onDrawArraysAfter() {
    if (mProgress>0){
        glUniform1f(mAlphaLocation,alpha);
        float scale = 1 +mProgress;
        scaleM(mMvpMatrix,0,scale,scale,scale);
        glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
}


void MagicSoulOutFilter::onInit() {
    GPUImageFilter::onInit();
    mMvpMatrixLocation = glGetUniformLocation(mGLProgId,"uMvpMatrix");
    mAlphaLocation = glGetUniformLocation(mGLProgId,"uAlpha");
    //开启颜色混合
    enableBlend(GL_SRC_ALPHA,GL_DST_ALPHA);
}

void MagicSoulOutFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}