#include "MagicShakeEffectFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicShakeEffectFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 抖动效果
 */
MagicShakeEffectFilter::MagicShakeEffectFilter(){

}

MagicShakeEffectFilter::MagicShakeEffectFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"soulout_v.glsl"), readShaderFromAsset(assetManager,"snakeeffect_f.glsl")),mMvpMatrix(new float[16]){

}

MagicShakeEffectFilter::~MagicShakeEffectFilter() {
    free(mMvpMatrix);
}

void MagicShakeEffectFilter::onDestroy() {
}

void MagicShakeEffectFilter::onDrawArraysPre() {
    mProgress = (float)mFrames/mMaxFrames;
    if (mProgress>1){
        mProgress = 0;
    }
    mFrames++;
    if (mFrames>mMaxFrames + mSkipFrames){
        mFrames = 0;
    }
    float scale= 1.0f+0.2f*mProgress;
    setIdentityM(mMvpMatrix,0);
    //设置方法百分比
    scaleM(mMvpMatrix,0,scale,scale,1.0f);
    glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
    //设置色值偏移量
    float textureCoordOffset = 0.01f *mProgress;
    glUniform1f(mTextureCoordOffsetLocation,textureCoordOffset);
}

void MagicShakeEffectFilter::onDrawArraysAfter() {

}


void MagicShakeEffectFilter::onInit() {
    GPUImageFilter::onInit();
    mMvpMatrixLocation = glGetUniformLocation(mGLProgId,"uMvpMatrix");
    mTextureCoordOffsetLocation = glGetUniformLocation(mGLProgId,"uTextureCoordOffset");
}

void MagicShakeEffectFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}