#include "MagicSoulOutFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicSoulOutFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 灵魂出窍
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
    //开启颜色混合
    glEnable(GL_BLEND);
    //透明度混合
    glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

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
        alpha = 0.2f - mProgress * 0.2f;
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
    //关闭颜色混合
    glDisable(GL_BLEND);
}


void MagicSoulOutFilter::onInit() {
    GPUImageFilter::onInit();
    mMvpMatrixLocation = glGetUniformLocation(mGLProgId,"uMvpMatrix");
    mAlphaLocation = glGetUniformLocation(mGLProgId,"uAlpha");
}

void MagicSoulOutFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}