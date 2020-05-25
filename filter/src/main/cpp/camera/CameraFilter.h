#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "utils/OpenglUtils.h"
#include "egl/GLBase.h"
#include "egl/EGLCore.h"
#include <android/asset_manager.h>
#include <filter/gpuimage/GpuImageFilter.h>
#include <filter/gpuimage/CameraInputFilterV2.h>
#include <android/native_window.h>

/**
 * cangwang 2018.12.1
 */
class CameraFilter:public GLBase{
public:
    CameraFilter(ANativeWindow *window);
    CameraFilter(ANativeWindow *window,AAssetManager *assetManager);
    ~CameraFilter();
    void setFilter(GPUImageFilter* gpuImageFilter);
    int create();
    void draw(GLfloat *matrix);
    void change(int width,int height);
    void stop();
    void setFilter(AAssetManager* assetManager);
    void setFilter(int type);
    void setBeautyLevel(int level);
    bool savePhoto(std::string saveFileAddress);
    EGLContext getCurrentContext();

protected:

private:
    GPUImageFilter *filter;
    CameraInputFilter *cameraInputFilter;
    AAssetManager *mAssetManager;
    ANativeWindow *mWindow;
    ANativeWindow *mVideoWindow;
    GLuint mTextureId;
    GLint mTextureLoc;
    GLint mMatrixLoc;
    GLfloat mMatrix[16];
    EGLCore *mEGLCore;
    std::MagicThreadPool *pool;
};

