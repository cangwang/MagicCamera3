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
    GLuint buffer=0;
    glActiveTexture(activeTextureUnit);
    mTextureId = get2DTextureID();
    char* texBuffer = (char*)malloc(sizeof(char*) * width *height*4);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texBuffer);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1,&buffer);
    mFrameBufferId = buffer;

    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);
    glGenRenderbuffers(1,&buffer);
    mRenderBufferId = buffer;

    glBindRenderbuffer(GL_RENDERBUFFER,mRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width,height);
}

void RenderBuffer::bind() {
    glViewport(0,0,mWidth,mHeight);
    checkGLError("glViewport");
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);
    checkGLError("glBindFramebuffer");
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mFrameBufferId,0);
    checkGLError("glFramebufferTexture2D");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,mRenderBufferId);
    checkGLError("glFramebufferRenderbuffer");
}

void RenderBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

int RenderBuffer::getTextureId() {
    return mTextureId;
}


RenderBuffer::~RenderBuffer() {

}
