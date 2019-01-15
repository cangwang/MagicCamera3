#include "MagicVerigoFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/utils/Matrix.h"

#define LOG_TAG "MagicVerigoFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/**
 * cangwang 2019.1.4
 * 幻觉  特效还没成功
 */
MagicVerigoFilter::MagicVerigoFilter(){

}

//MagicVerigoFilter::MagicVerigoFilter(AAssetManager *assetManager)
//    : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"verigo_f.glsl")),mAssetManager(assetManager){
//
//}
//
//MagicVerigoFilter::~MagicVerigoFilter() {
//    glDeleteTextures(1,&mLutTexture);
//}
//
//void MagicVerigoFilter::onDestroy() {
//
//}
//
//void MagicVerigoFilter::onDrawArraysPre() {
//    if(mLutTexture !=0){
//        glActiveTexture(GL_TEXTURE3);
//        glBindTexture(GL_TEXTURE_2D,mLutTexture);
//        glUniform1i(mLutTextureLocation,3);
//    }
//    if (mLastTexture !=0){
//        glActiveTexture(GL_TEXTURE4);
//        glBindTexture(GL_TEXTURE_2D,mLastTexture);
//        glUniform1i(mLastTextureLocation,4);
//    }
//}
//
//void MagicVerigoFilter::onDrawArraysAfter(GLuint textureId) {
//    if (mLutTexture != 0){
//        glActiveTexture(GL_TEXTURE3);
//        glBindTexture(GL_TEXTURE_2D,mLutTexture);
//        glActiveTexture(GL_TEXTURE0);
//    }
//    if (mLastTexture !=0){
//        glActiveTexture(GL_TEXTURE4);
//        glBindTexture(GL_TEXTURE_2D,mLastTexture);
//        glActiveTexture(GL_TEXTURE0);
//    }
//    mLastTexture = textureId;
//}
//
//
//void MagicVerigoFilter::onInit() {
//    GPUImageFilter::onInit();
//    mLutTextureLocation = glGetUniformLocation(mGLProgId,"lookupTable");
//    mLastTextureLocation = glGetUniformLocation(mGLProgId,"inputTextureLast");
//}
//
//void MagicVerigoFilter::onInitialized() {
//    GPUImageFilter::onInitialized();
//    mLutTexture = loadTextureFromAssets(mAssetManager,"lookup_vertigo.png");
//}

MagicVerigoFilter::MagicVerigoFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"common_f.glsl")),
          mAssetManager(assetManager),mRenderBuffer(nullptr){

}

MagicVerigoFilter::~MagicVerigoFilter() {
    free(mRenderBuffer);
    free(mRenderBuffer2);
    free(mRenderBuffer3);
}

void MagicVerigoFilter::onDestroy() {

}

void MagicVerigoFilter::onDrawArraysPre() {
    mRenderBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);
}

void MagicVerigoFilter::onDrawArraysAfter() {
    mRenderBuffer->unbind();
    glClear(GL_COLOR_BUFFER_BIT);
    drawCurrentFrame();
    mRenderBuffer3->bind();
    drawCurrentFrame();
    mRenderBuffer3->unbind();
    mRenderBuffer2->bind();
    drawToBuffer();
    mRenderBuffer2->unbind();
    mFirst = false;
}


void MagicVerigoFilter::onInit() {
    GPUImageFilter::onInit();

}

void MagicVerigoFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    mLastFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"common_f.glsl")->c_str());
    mCurrentFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"verigo_f.glsl")->c_str());
    mLutTexture = loadTextureFromAssetsRepeat(mAssetManager,"lookup_vertigo.png");
}

void MagicVerigoFilter::onInputSizeChanged(const int width, const int height) {
    mInputWidth = width;
    mInputHeight = height;
    mRenderBuffer  = new RenderBuffer(GL_TEXTURE8,width,height);
    mRenderBuffer2 = new RenderBuffer(GL_TEXTURE9,width,height);
    mRenderBuffer3 = new RenderBuffer(GL_TEXTURE10,width,height);
}

void MagicVerigoFilter::drawToBuffer() {
//    glUseProgram(mLastFrameProgram);
//    setup(mLastFrameProgram,new GLint[1]{mRenderBuffer3->getTextureId()});

    glUseProgram(mLastFrameProgram);
    GLint position = glGetAttribLocation(mLastFrameProgram,"position");
    GLint texcoord = glGetAttribLocation(mLastFrameProgram,"inputTextureCoordinate");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,getVertexBuffer());
    glEnableVertexAttribArray(texcoord);

    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,getTextureBuffer());
    GLint textureLocation0 = glGetUniformLocation(mLastFrameProgram,"inputImageTexture");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,mRenderBuffer3->getTextureId());
    glUniform1f(textureLocation0,3);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_2D,mRenderBuffer3->getTextureId());
//    glActiveTexture(GL_TEXTURE0);
}

void MagicVerigoFilter::drawCurrentFrame() {
    int textureId = mRenderBuffer->getTextureId();
//    setup(mCurrentFrameProgram,new GLint[2]{textureId,mFirst?mRenderBuffer2->getTextureId():mLutTexture});

    glUseProgram(mCurrentFrameProgram);
    GLint position = glGetAttribLocation(mCurrentFrameProgram,"position");
    GLint texcoord = glGetAttribLocation(mCurrentFrameProgram,"inputTextureCoordinate");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,getVertexBuffer());
    glEnableVertexAttribArray(texcoord);

    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,getTextureBuffer());
    GLint textureLocation0 = glGetUniformLocation(mCurrentFrameProgram,"inputImageTexture");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,textureId);
    glUniform1f(textureLocation0,3);

    GLint textureLocation1 = glGetUniformLocation(mCurrentFrameProgram,"inputTextureLast");
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,mFirst?mRenderBuffer2->getTextureId():mLutTexture);
    glUniform1f(textureLocation1,4);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

//    glActiveTexture(GL_TEXTURE3);
//    glBindTexture(GL_TEXTURE_2D,textureId);
//    glActiveTexture(GL_TEXTURE0);
//
//    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_2D,mFirst?mRenderBuffer2->getTextureId():mLutTexture);
//    glActiveTexture(GL_TEXTURE0);
}

void MagicVerigoFilter::setup(GLuint programId, GLint* textureId) {
    glUseProgram(programId);
    GLint position = glGetAttribLocation(programId,"position");
    GLint texcoord = glGetAttribLocation(programId,"inputTextureCoordinate");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,getVertexBuffer());
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,getTextureBuffer());

}