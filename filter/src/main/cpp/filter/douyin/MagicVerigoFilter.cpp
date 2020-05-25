#include "MagicVerigoFilter.h"
#include "utils/OpenglUtils.h"
#include "utils/Matrix.h"

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

void MagicVerigoFilter::onDrawPrepare() {
    //绑定纹理
    mRenderBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT);
}

void MagicVerigoFilter::onDrawArraysAfter() {
    //将摄像头的数据保存到mRenderBuffer的fbo中
    mRenderBuffer->unbind();

    //在顶层画帧，真正画显示的画面
    drawCurrentFrame();

    mRenderBuffer3->bind();
    //绘制当前帧
    drawCurrentFrame();
    //将当前帧保存到生成mRenderBuffer3的fbo
    mRenderBuffer3->unbind();

    mRenderBuffer2->bind();
    //使用mRenderBuffer的fbo，再绘制mRenderBuffer2的纹理fbo中
    drawToBuffer();
    //生成mRenderBuffer2的fbo，用于下一帧的绘制
    mRenderBuffer2->unbind();
    mFirst = false;
}


void MagicVerigoFilter::onInit() {
    GPUImageFilter::onInit();
}

void MagicVerigoFilter::onInitialized() {
    GPUImageFilter::onInitialized();
    //用于上一帧数据，用于下一帧
    mLastFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"common_f.glsl")->c_str());
    //此帧使用的绘制program
    mCurrentFrameProgram = loadProgram(readShaderFromAsset(mAssetManager,"nofilter_v.glsl")->c_str(),readShaderFromAsset(mAssetManager,"verigo_f2.glsl")->c_str());
    //Lut纹理
    mLutTexture = loadTextureFromAssetsRepeat(mAssetManager,"lookup_vertigo.png");
}

void MagicVerigoFilter::onInputSizeChanged(const int width, const int height) {
    mScreenWidth = width;
    mScreenHeight = height;
    //建立三个fbo纹理
    mRenderBuffer  = new RenderBuffer(GL_TEXTURE8,width,height);
    mRenderBuffer2 = new RenderBuffer(GL_TEXTURE9,width,height);
    mRenderBuffer3 = new RenderBuffer(GL_TEXTURE10,width,height);
}

void MagicVerigoFilter::drawToBuffer() {

    glUseProgram(mLastFrameProgram);
    GLint position = glGetAttribLocation(mLastFrameProgram,"position");
    GLint texcoord = glGetAttribLocation(mLastFrameProgram,"inputTextureCoordinate");
    GLint matrix = glGetUniformLocation(mLastFrameProgram,"mvpMatrix");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_COORDS);
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_TEX_COORDS);
    glUniformMatrix4fv(matrix,1,GL_FALSE,NONE_MATRIX);

    //保存到mRenderBuffer3中的fbo，用于下一帧
    GLint textureLocation0 = glGetUniformLocation(mLastFrameProgram,"inputImageTexture");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,mRenderBuffer3->getTextureId());
    glUniform1i(textureLocation0,3);

    glClear(GL_COLOR_BUFFER_BIT);
    //绘制
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    //关闭纹理
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,mRenderBuffer3->getTextureId());
    glActiveTexture(GL_TEXTURE0);
}

void MagicVerigoFilter::drawCurrentFrame() {
    glUseProgram(mCurrentFrameProgram);
    GLint position = glGetAttribLocation(mCurrentFrameProgram,"position");
    GLint texcoord = glGetAttribLocation(mCurrentFrameProgram,"inputTextureCoordinate");
    GLint matrix = glGetUniformLocation(mCurrentFrameProgram,"mvpMatrix");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_COORDS);
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_TEX_COORDS);
    glUniformMatrix4fv(matrix,1,GL_FALSE,NONE_MATRIX);

    //绑定此帧纹理到mRenderBuffer（摄像头原始数据）
    GLint textureLocation0 = glGetUniformLocation(mCurrentFrameProgram,"inputImageTexture");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,mRenderBuffer->getTextureId());
    glUniform1i(textureLocation0,3);

    //如果是第一帧绑定为mRenderBuffer的fbo，除外绑定mRenderBuffer2的fbo
    GLint textureLocation1 = glGetUniformLocation(mCurrentFrameProgram,"inputTextureLast");
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,mFirst ? mRenderBuffer->getTextureId() : mRenderBuffer2->getTextureId());
    glUniform1i(textureLocation1,4);

    //使用Lut纹理
    GLint textureLocation2 = glGetUniformLocation(mCurrentFrameProgram,"lookupTable");
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,mLutTexture);
    glUniform1i(textureLocation2,5);

    glClear(GL_COLOR_BUFFER_BIT);
    //画图
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glDisableVertexAttribArray(position);
    glDisableVertexAttribArray(texcoord);

    //关闭纹理
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,mRenderBuffer->getTextureId());
    glActiveTexture(GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D,mFirst ? mRenderBuffer->getTextureId() : mRenderBuffer2->getTextureId());
    glActiveTexture(GL_TEXTURE0);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D,mLutTexture);
    glActiveTexture(GL_TEXTURE0);
}

void MagicVerigoFilter::setup(GLuint programId, GLint* textureId) {
    glUseProgram(programId);
    GLint position = glGetAttribLocation(programId,"position");
    GLint texcoord = glGetAttribLocation(programId,"inputTextureCoordinate");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_COORDS);
    glEnableVertexAttribArray(texcoord);
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,0,FULL_RECTANGLE_TEX_COORDS);

}