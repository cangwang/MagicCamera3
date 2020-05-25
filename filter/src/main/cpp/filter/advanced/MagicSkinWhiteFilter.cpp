#include "MagicSkinWhiteFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicSkinWhiteFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cangwang 2018.12.1
 */
MagicSkinWhiteFilter::MagicSkinWhiteFilter(){

}

MagicSkinWhiteFilter::MagicSkinWhiteFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"skinwhiten.glsl")),mToneCurveTexture(0){

}

MagicSkinWhiteFilter::~MagicSkinWhiteFilter() {

}

void MagicSkinWhiteFilter::onDestroy() {
    glDeleteTextures(1,&mToneCurveTexture);
}

void MagicSkinWhiteFilter::onDrawArraysPre() {
    glUniform1f(mTexelWidthUniformLocation,mChangeWidth);
    glUniform1f(mTexelHeightUniformLocation,mChangeHeight);
    if(this->mToneCurveTexture !=0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glUniform1i(this->mToneCurveTextureUniformLocation,3);
    }
}

void MagicSkinWhiteFilter::onDrawArraysAfter() {
    if (this->mToneCurveTexture != 0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
        glActiveTexture(GL_TEXTURE0);
    }
}


void MagicSkinWhiteFilter::onInit() {
    GPUImageFilter::onInit();
    mToneCurveTextureUniformLocation = glGetUniformLocation(mGLProgId,"curve");
    mTexelWidthUniformLocation = glGetUniformLocation(mGLProgId, "texelWidthOffset");
    mTexelHeightUniformLocation = glGetUniformLocation(mGLProgId, "texelHeightOffset");
}

void MagicSkinWhiteFilter::onInitialized() {
    GPUImageFilter::onInitialized();

    glGenTextures(1,&mToneCurveTexture);
    glBindTexture(GL_TEXTURE_2D,mToneCurveTexture);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //c++中没有byte类型，用unsigned char代替
    unsigned char* arrayOfByte = (unsigned char*)malloc(sizeof(char)*2048);
    unsigned char arrayOfInt1[] = { 95, 95, 96, 97, 97, 98, 99, 99, 100, 101, 101, 102, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109, 110, 111, 111, 112, 113, 113, 114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 124, 125, 126, 127, 127, 128, 129, 129, 130, 131, 131, 132, 133, 133, 134, 135, 136, 136, 137, 138, 138, 139, 140, 140, 141, 142, 143, 143, 144, 145, 145, 146, 147, 147, 148, 149, 149, 150, 151, 152, 152, 153, 154, 154, 155, 156, 156, 157, 158, 159, 159, 160, 161, 161, 162, 163, 163, 164, 165, 165, 166, 167, 168, 168, 169, 170, 170, 171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 179, 180, 181, 181, 182, 183, 184, 184, 185, 186, 186, 187, 188, 188, 189, 190, 191, 191, 192, 193, 193, 194, 195, 195, 196, 197, 197, 198, 199, 200, 200, 201, 202, 202, 203, 204, 204, 205, 206, 207, 207, 208, 209, 209, 210, 211, 211, 212, 213, 213, 214, 215, 216, 216, 217, 218, 218, 219, 220, 220, 221, 222, 223, 223, 224, 225, 225, 226, 227, 227, 228, 229, 229, 230, 231, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 239, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    unsigned char arrayOfInt2[] = { 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 28, 29, 29, 30, 29, 31, 31, 31, 31, 32, 32, 33, 33, 34, 34, 34, 34, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 42, 42, 43, 43, 44, 44, 45, 45, 45, 46, 47, 47, 48, 48, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 63, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 90, 91, 93, 94, 95, 96, 97, 98, 100, 101, 103, 104, 105, 107, 108, 110, 111, 113, 115, 116, 118, 119, 120, 122, 123, 125, 127, 128, 130, 132, 134, 135, 137, 139, 141, 143, 144, 146, 148, 150, 152, 154, 156, 158, 160, 163, 165, 167, 169, 171, 173, 175, 178, 180, 182, 185, 187, 189, 192, 194, 197, 199, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 236, 239, 241, 245, 247, 250, 252, 255 };
    for (int i = 0; i < 256; ++i) {
        arrayOfByte[i*4]= arrayOfInt1[i];
        arrayOfByte[(1+i*4)]= arrayOfInt1[i];
        arrayOfByte[(2+i*4)]= arrayOfInt2[i];
        arrayOfByte[(3+i*4)]= static_cast<unsigned char>(-1);
    }
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,256,1,0,GL_RGBA,GL_UNSIGNED_BYTE,arrayOfByte);
}

void MagicSkinWhiteFilter::onInputSizeChanged(const int width, const int height) {
    mChangeWidth = 1.0f/width;
    mChangeHeight = 1.0f/height;

}