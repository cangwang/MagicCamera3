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

    GLboolean buildContext(ANativeWindow *window);

    void setPresentationTime(long nsecs);

    void swapBuffer();

    void release();

protected:

private:
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
    EGL_PRESENTATION_TIME_ANDROIDPROC eglPresentationTimeANDROID = NULL;
};

#endif
