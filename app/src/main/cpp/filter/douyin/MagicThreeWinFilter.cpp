#include "MagicThreeWinFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicThreeWinFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 三重影像
 */
MagicThreeWinFilter::MagicThreeWinFilter(){

}

MagicThreeWinFilter::MagicThreeWinFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"soulout_v.glsl"), readShaderFromAsset(assetManager,"three_win.glsl")),mMvpMatrix(new float[16]){

}

MagicThreeWinFilter::~MagicThreeWinFilter() {
    free(mMvpMatrix);
}

void MagicThreeWinFilter::onDestroy() {
}

void MagicThreeWinFilter::onDrawArraysPre() {
    setIdentityM(mMvpMatrix, 0);
    glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
}

void MagicThreeWinFilter::onDrawArraysAfter() {

}


void MagicThreeWinFilter::onInit() {
    GPUImageFilter::onInit();
    mMvpMatrixLocation = glGetUniformLocation(mGLProgId,"uMvpMatrix");
}

void MagicThreeWinFilter::onInitialized() {
    GPUImageFilter::onInitialized();

}