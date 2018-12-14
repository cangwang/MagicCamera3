#include "MagicSweetsFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicCoolFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cangwang 2018.12.1
 */
MagicSweetsFilter::MagicSweetsFilter(){

}

MagicSweetsFilter::MagicSweetsFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"sweets.glsl")),
      mToneCurveTexture(0),
      mMaskGery1TextureId(0){

}

MagicSweetsFilter::~MagicSweetsFilter() {

}

void MagicSweetsFilter::onDestroy() {
    glDeleteTextures(1, &mToneCurveTexture);
    glDeleteTextures(1, &mMaskGery1TextureId);
}

void MagicSweetsFilter::onDrawArraysPre() {
    if(this->mToneCurveTexture !=0){
        glActiveTexture(GL_TEXTURE3);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glUniform1i(mToneCurveTextureUniformLocation,3);
    }

    if(this->mMaskGery1TextureId !=0){
        glActiveTexture(GL_TEXTURE4);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,mMaskGery1TextureId);
        glUniform1i(mMaskGery1UniformLocation,4);
    }
}

void MagicSweetsFilter::onDrawArraysAfter() {

    if (this->mToneCurveTexture != 0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glActiveTexture(GL_TEXTURE0);
    }
    if (this->mMaskGery1TextureId != 0){
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,mMaskGery1TextureId);
        glActiveTexture(GL_TEXTURE0);
    }
}

void MagicSweetsFilter::onInit() {
    GPUImageFilter::onInit();
    mToneCurveTextureUniformLocation = glGetUniformLocation(mGLProgId,"curve");
    mMaskGery1UniformLocation = glGetUniformLocation(mGLProgId,"grey1Frame");
    mLowPerformanceUniformLocation = glGetUniformLocation(mGLProgId,"lowPerformance");
    glUniform1f(mLowPerformanceUniformLocation,1);
}

void MagicSweetsFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    glGenTextures(1, &mToneCurveTexture);
    glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //c++中没有byte类型，用unsigned char代替
    unsigned char* arrayOfByte = (unsigned char*)malloc(sizeof(char)*1024);
    unsigned char arrayOfInt[] = { 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 10, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 25, 26, 27, 28, 29, 30, 30, 31, 32, 33, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 92, 93, 94, 95, 96, 98, 99, 100, 101, 103, 104, 105, 106, 108, 109, 110, 111, 113, 114, 115, 116, 118, 119, 120, 121, 123, 124, 125, 126, 128, 129, 130, 132, 133, 134, 135, 137, 138, 139, 140, 142, 143, 144, 145, 147, 148, 149, 150, 152, 153, 154, 155, 157, 158, 159, 160, 161, 163, 164, 165, 166, 167, 169, 170, 171, 172, 173, 174, 176, 177, 178, 179, 180, 181, 182, 183, 184, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 209, 210, 211, 212, 213, 214, 215, 216, 217, 217, 218, 219, 220, 221, 222, 222, 223, 224, 225, 226, 227, 227, 228, 229, 230, 230, 231, 232, 233, 234, 234, 235, 236, 237, 237, 238, 239, 240, 240, 241, 242, 243, 243, 244, 245, 246, 246, 247, 248, 248, 249, 250, 251, 251, 252, 253, 254, 254, 255 };
    for (int i = 0; i < 256; i++){
        arrayOfByte[(i * 4)] = arrayOfInt[i];
        arrayOfByte[(1 + i * 4)] = arrayOfInt[i];
        arrayOfByte[(2 + i * 4)] = arrayOfInt[i];
        arrayOfByte[(3 + i * 4)] = static_cast<unsigned char>(-1);
    }

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,1,0,GL_RGBA,GL_UNSIGNED_BYTE,arrayOfByte);
    mMaskGery1TextureId=loadTextureFromAssets(mAssetManager,"rise_mask2.jpg");

}
