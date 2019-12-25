#include "RenderBuffer.h"
#include "utils/OpenglUtils.h"

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
    //激活纹理插槽
    glActiveTexture(activeTextureUnit);
//    mTextureId = get2DTextureRepeatID();
    //纹理id
    mTextureId = get2DTextureID();
//    unsigned char* texBuffer = (unsigned char*)malloc(sizeof(unsigned char*) * width * height * 4);
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,texBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    //生成fb的id
    glGenFramebuffers(1,&mFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);
    //生成渲染缓冲区id
    glGenRenderbuffers(1,&mRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER,mRenderBufferId);
    //指定存储在 renderbuffer 中图像的宽高以及颜色格式，并按照此规格为之分配存储空间
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width,height);
    //复位
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void RenderBuffer::bind() {
    //清空视口
    glViewport(0,0,mWidth,mHeight);
    //绑定fb的纹理id
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBufferId);
    //绑定2D纹理关联到fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mTextureId,0);
    //绑定fo纹理到渲染缓冲区对象
    glBindRenderbuffer(GL_RENDERBUFFER,mRenderBufferId);
    //将渲染缓冲区作为深度缓冲区附加到fbo
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,mRenderBufferId);
    //检查fbo的状态
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        ALOGE("framebuffer error");
    }
}

void RenderBuffer::unbind() {
    //移除绑定
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
//    glActiveTexture(GL_TEXTURE0);
}

GLuint RenderBuffer::getTextureId() {
    return mTextureId;
}


RenderBuffer::~RenderBuffer() {
    //释放纹理资源
    glDeleteBuffers(1,&mTextureId);
    glDeleteFramebuffers(1,&mFrameBufferId);
    glDeleteRenderbuffers(1,&mRenderBufferId);
}
