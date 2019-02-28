#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES3/gl3.h>

#ifndef _EGLCore
#define _EGLCore

/**
 * cangwang 2018.12.1
 */
typedef EGLBoolean (EGLAPIENTRYP EGL_PRESENTATION_TIME_ANDROIDPROC)(EGLDisplay display, EGLSurface surface, khronos_stime_nanoseconds_t time);
 class EGLCore{
public:
    EGLCore();

    ~EGLCore();

    GLboolean buildContext(ANativeWindow *window,EGLContext context);
    GLboolean buildVideoContext(ANativeWindow *window, EGLContext context);

    void makeCurrent();

    void setPresentationTime(long nsecs);

    void swapBuffer();

    void release();

    EGLContext mContext;

protected:

private:
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGL_PRESENTATION_TIME_ANDROIDPROC eglPresentationTimeANDROID = NULL;
};

#endif
