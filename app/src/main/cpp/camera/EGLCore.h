#include <EGL/egl.h>
#include <GLES3/gl3.h>

#ifndef _EGLCore
#define _EGLCore

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
