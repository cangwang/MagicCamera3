#include "MagicVerigoFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicVerigoFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/**
 * cangwang 2019.1.4
 * 幻觉
 */
MagicVerigoFilter::MagicVerigoFilter(){

}

MagicVerigoFilter::MagicVerigoFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"verigo_f.glsl")),mAssetManager(assetManager){

}

MagicVerigoFilter::~MagicVerigoFilter() {
    glDeleteTextures(1,&mLutTexture);
}

void MagicVerigoFilter::onDestroy() {

}

void MagicVerigoFilter::onDrawArraysPre() {
    if(mLutTexture !=0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mLutTexture);
        glUniform1i(mLutTextureLocation,3);
    }
    if (mLastTexture !=0){
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,mLastTexture);
        glUniform1i(mLastTextureLocation,4);
    }
}

void MagicVerigoFilter::onDrawArraysAfter(GLuint textureId) {
    if (mLutTexture != 0){
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,mLutTexture);
        glActiveTexture(GL_TEXTURE0);
    }
    if (mLastTexture !=0){
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,mLastTexture);
        glActiveTexture(GL_TEXTURE0);
    }
    mLastTexture = textureId;
}


void MagicVerigoFilter::onInit() {
    GPUImageFilter::onInit();
    mLutTextureLocation = glGetUniformLocation(mGLProgId,"lookupTable");
    mLastTextureLocation = glGetUniformLocation(mGLProgId,"inputTextureLast");
}

void MagicVerigoFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    mLutTexture = loadTextureFromAssets(mAssetManager,"lookup_vertigo.png");
}