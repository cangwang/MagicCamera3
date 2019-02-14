#include "RenderBuffer.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "RenderBuffer"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


/**
 * cangwang 2019.1.4
 * 幻觉帧图
 */
RenderBuffer::RenderBuffer(){

}

RenderBuffer::RenderBuffer(GLenum activeTextureUnit, int width, int height) {
    mWidth = width;
    mHeight = height;
    glActiveTexture(activeTextureUnit);
//    mTextureId = get2DTextureRepeatID();
    mTextureId = get2DTextureID();
//    unsigned char* texBuffer = (unsigned char*)malloc(sizeof(unsigned char*) * width * height * 4);
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1,&mFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);

    glGenRenderbuffers(1,&mRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER,mRenderBufferId);
    //指定存储在 renderbuffer 中图像的宽高以及颜色格式，并按照此规格为之分配存储空间
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width,height);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void RenderBuffer::bind() {
    checkGLError("RenderBuffer");
    glViewport(0,0,mWidth,mHeight);
    checkGLError("glViewport");
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);
    checkGLError("glBindFramebuffer");
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mTextureId,0);
    checkGLError("glFramebufferTexture2D");
    glBindRenderbuffer(GL_RENDERBUFFER,mRenderBufferId);
    //将渲染缓冲区作为深度缓冲区附加到fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,mRenderBufferId);
    checkGLError("glFramebufferRenderbuffer");
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        ALOGE("framebuffer error");
    }
}

void RenderBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
//    glActiveTexture(GL_TEXTURE0);
}

GLuint RenderBuffer::getTextureId() {
    return mTextureId;
}


RenderBuffer::~RenderBuffer() {
    glDeleteBuffers(1,&mTextureId);
    glDeleteFramebuffers(1,&mFrameBufferId);
    glDeleteRenderbuffers(1,&mRenderBufferId);
}
