#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/utils/OpenglUtils.h"
#include "src/main/cpp/egl/EGLCore.h"
#include "src/main/cpp/egl/GLBase.h"
#include "ImageInput.h"
#include <android/asset_manager.h>
#include <src/main/cpp/filter/gpuimage/CameraInputFilterV2.h>
#include <android/native_window.h>
#include <src/main/cpp/filter/advanced/MagicBeautyFilter.h>

/**
 * cangwang 2018.12.1
 */
class ImageFilter:public GPUImageFilter{
public:
    ImageFilter();
    ImageFilter(ANativeWindow *window,AAssetManager *assetManager,std::string path,int degree);
    ~ImageFilter();
    void setFilter(GPUImageFilter* gpuImageFilter);
    int create();
    void draw(GLfloat *matrix);
    void change(int width,int height);
    void stop();
    void setFilter(AAssetManager* assetManager);
    void setFilter(int type);
    void setBeautyLevel(int level);
    bool saveImage(std::string saveFileAddress);
    void setMatrix(int width,int height);

protected:

private:
    GPUImageFilter *filter;
    ImageInput *imageInput = nullptr;
    AAssetManager *mAssetManager;
    ANativeWindow *mWindow;
    GLuint mTextureId;
    GLint mTextureLoc;
    GLint mMatrixLoc;
    GLfloat mMatrix[16];
    EGLCore *mEGLCore;
    std::string imgPath;
    MagicBeautyFilter* beautyFilter;
    int degree;
    GLuint mFrameBuffer;
    GLuint mFrameBufferTextures;
    int mFrameWidth = -1;
    int mFrameHeight = -1;
    float *mvpMatrix;
};

