#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/utils/OpenglUtils.h"
#include "GLBase.h"
#include "EGLCore.h"
#include <android/asset_manager.h>
#include <src/main/cpp/filter/gpuimage/GpuImageFilter.h>
#include <src/main/cpp/filter/gpuimage/CameraInputFilter.h>
#include <android/native_window.h>

class CameraFilter:public GLBase{
public:
    CameraFilter(ANativeWindow *window);
    CameraFilter(ANativeWindow *window,AAssetManager *assetManager);
    ~CameraFilter();
    void setAssetManager(AAssetManager *assetManager);
    int create();
    void draw(GLfloat *matrix);
    void change(int width,int height);
    void stop();
    void setFilter(AAssetManager* assetManager);

protected:

private:
    GPUImageFilter *filter;
    CameraInputFilter *cameraInputFilter;
    AAssetManager *mAssetManager;
    ANativeWindow *mWindow;
    GLuint mTextureId;
    GLint mTextureLoc;
    GLint mMatrixLoc;
    GLfloat mMatrix[16];
    EGLCore *mEGLCore;
};

