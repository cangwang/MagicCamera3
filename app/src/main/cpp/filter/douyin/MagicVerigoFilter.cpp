#include "MagicVerigoFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicVerigoFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2019.1.4
 * 幻觉
 */
MagicVerigoFilter::MagicVerigoFilter(){

}

MagicVerigoFilter::MagicVerigoFilter(AAssetManager *assetManager)
    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"soulout_v.glsl"), readShaderFromAsset(assetManager,"soulout_f.glsl")),mAssetManager(assetManager){

}

MagicVerigoFilter::~MagicVerigoFilter() {

}

void MagicVerigoFilter::onDestroy() {
}

void MagicVerigoFilter::onDrawArraysPre() {


}

void MagicVerigoFilter::onDrawArraysAfter() {
}


void MagicVerigoFilter::onInit() {
    GPUImageFilter::onInit();

}

void MagicVerigoFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    mRenderBuffer  = new RenderBuffer(GL_TEXTURE3,mInputWidth,mInputHeight);
    mRenderBuffer2 = new RenderBuffer(GL_TEXTURE4,mInputWidth,mInputHeight);
    mRenderBuffer3 = new RenderBuffer(GL_TEXTURE5,mInputWidth,mInputHeight);
    mLastFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"")->c_str());
    mCurrentFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"")->c_str());
    mLutTexture = get2DTextureID();
    loadTextureFromAssets(mAssetManager,"lookup_vertigo.png");
}

void MagicVerigoFilter::drawToBuffer() {
    glUseProgram(mLastFrameProgram);
    setup(mLastFrameProgram,new GLint[1]{mRenderBuffer3->getTextureId()});
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

void MagicVerigoFilter::drawCurrentFrame() {
    glUseProgram(mCurrentFrameProgram);
    int textureId = mRenderBuffer->getTextureId();
    setup(mCurrentFrameProgram,new GLint[2]{textureId,mFirst?mRenderBuffer2->getTextureId():mLutTexture});
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

void MagicVerigoFilter::setup(GLuint programId, GLint* textureId) {
    glUseProgram(programId);
    GLint position = glGetAttribLocation(programId,"position");
    GLint texcoord = glGetAttribLocation(programId,"inputTextureCoordinate");
    glEnableVertexAttribArray(position);
//    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,mRe)
}