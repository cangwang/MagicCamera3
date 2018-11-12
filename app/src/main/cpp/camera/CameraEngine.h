#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/bitmap/BitmapOperation.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <android/asset_manager.h>
#include "EGLCore.h"
#include "GLBase.h"

#define LOG_TAG "CameraEngine"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

class CameraEngine:public GLBase{
public:
    GPUImageFilter filter;
    CameraEngine(ANativeWindow *window);
    ~CameraEngine();
    void setAssetManager(AAssetManager *assetManager);
    int create();
    void draw();
    void stop();

protected:

private:
    AAssetManager *mAssetManager;
    ANativeWindow *mWindow;
    GLuint mTextureId;
    GLint mTextureLoc;
    GLint mMatrixLoc;
    GLfloat mMatrix[16];
    EGLCore *mEGLCore;
};

