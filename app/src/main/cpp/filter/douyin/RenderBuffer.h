#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>

/**
 * cangwang 2019.1.4
 */
class RenderBuffer{

public:
    RenderBuffer();
    RenderBuffer(GLenum activeTextureUnit,int width,int height);
    ~RenderBuffer();
    void bind();
    void unbind();
    int getTextureId();

protected:

private:
    GLint mTextureId;
    GLint mRenderBufferId;
    GLint mFrameBufferId;
    int mWidth;
    int mHeight;
};