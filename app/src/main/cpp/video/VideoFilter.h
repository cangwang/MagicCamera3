#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/egl/GLBase.h"
#include "src/main/cpp/egl/EGLCore.h"
#include <android/asset_manager.h>
#include <src/main/cpp/filter/gpuimage/GpuImageFilter.h>
#include <src/main/cpp/filter/gpuimage/CameraInputFilterV2.h>
#include <android/native_window.h>

/**
 * cangwang 2018.12.1
 */
class VideoFilter:public GLBase{
public:
    VideoFilter(ANativeWindow *window);
    VideoFilter(ANativeWindow *window,AAssetManager *assetManager);
    ~VideoFilter();
    void setFilter(GPUImageFilter* gpuImageFilter);
    int create(int textureId,EGLContext eglContext);
    void draw(GLfloat *matrix,long long time);
    void change(int width,int height);
    void stop();
    void setFilter(AAssetManager* assetManager);
    void setFilter(int type);
    void setBeautyLevel(int level);
    bool savePhoto(std::string saveFileAddress);

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

