#include "MagicScaleFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicScaleFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 放大
 */
MagicScaleFilter::MagicScaleFilter(){

}

MagicScaleFilter::MagicScaleFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"soulout_v.glsl"), readShaderFromAsset(assetManager,"soulout_f.glsl")),mMvpMatrix(new float[16]){

}

MagicScaleFilter::~MagicScaleFilter() {
    free(mMvpMatrix);
}

void MagicScaleFilter::onDestroy() {
}

void MagicScaleFilter::onDrawArraysPre() {
    if (mFrames <= mMiddleFrames){ //根据中间帧为间隔，放大过程
        mProgress = mFrames * 1.0f /mMiddleFrames;
    } else{  //缩小过程
        mProgress = 2.0f - mFrames * 1.0f /mMiddleFrames;
    }
    setIdentityM(mMvpMatrix, 0);
    float scale = 1.0f+0.3f*mProgress;
    //正交矩阵放大
    scaleM(mMvpMatrix,0,scale,scale,scale);
    glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
    mFrames++;
    if (mFrames>mMaxFrames){
        mFrames = 0;
    }
}

void MagicScaleFilter::onDrawArraysAfter() {

}


void MagicScaleFilter::onInit() {
    GPUImageFilter::onInit();
    mMvpMatrixLocation = glGetUniformLocation(mGLProgId,"uMvpMatrix");
    mAlphaLocation = glGetUniformLocation(mGLProgId,"uAlpha");
}

void MagicScaleFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}