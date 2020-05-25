#include "MagicSoulOutFilter.h"
#include "utils/OpenglUtils.h"
#include "utils/Matrix.h"

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
    //存在两个图层,开启颜色混合
    glEnable(GL_BLEND);
    //透明度混合
    // 表示源颜色乘以自身的alpha 值，目标颜色乘目标颜色值混合，如果不开启，直接被目标的画面覆盖
    glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
    //最大帧数mMaxFrames为15，灵魂出窍只显示15帧，设定有8帧不显示
    mProgress = (float)mFrames/mMaxFrames;
    //当progress大于1后置0
    if (mProgress > 1.0f){
        mProgress = 0;
    }

    mFrames++;
    //skipFrames为8，23帧后置为0
    if (mFrames > mMaxFrames + mSkipFrames){
        mFrames = 0;
    }
    //setIdentityM函数移植于java中Matrix.setIdentity，初始化矩阵全置为0
    setIdentityM(mMvpMatrix,0);
    //第一帧没有放大，直接设置单位矩阵
    glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
    //透明度为1
    float backAlpha = 1;
    if (mProgress > 0){ //如果是灵魂出窍效果显示中
        //计算出窍时透明度值
        alpha = 0.2f - mProgress * 0.2f;
        backAlpha = 1 - alpha;
    }
    //设置不显示灵魂出窍效果时，背景不透明度，不然会黑色
    glUniform1f(mAlphaLocation,backAlpha);
}

void MagicSoulOutFilter::onDrawArraysAfter() {
    if (mProgress>0){ //如果是灵魂出窍效果显示中
        glUniform1f(mAlphaLocation,alpha);
        //设置放大值
        float scale = 1 + mProgress;
        //设置正交矩阵放大
        scaleM(mMvpMatrix,0,scale,scale,scale);
        //设置到shader里面
        glUniformMatrix4fv(mMvpMatrixLocation,1,GL_FALSE,mMvpMatrix);
        //画出灵魂出校效果
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