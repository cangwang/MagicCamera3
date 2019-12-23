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
    GLuint getTextureId();

protected:

private:
    GLuint mTextureId;
    GLuint mRenderBufferId;
    GLuint mFrameBufferId;
    int mWidth;
    int mHeight;
};