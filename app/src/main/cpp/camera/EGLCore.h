#include <EGL/egl.h>
#include <GLES3/gl3.h>

#ifndef _EGLCore
#define _EGLCore

/**
 * cangwang 2018.12.1
 */
class EGLCore{
public:
    EGLCore();

    ~EGLCore();

    GLboolean buildContext(ANativeWindow *window);

    void swapBuffer();

    void release();

protected:

private:
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
};

#endif
