#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <string>
#include <src/main/cpp/filter/gpuimage/GpuImageFilter.h>

#ifndef _ImageInput
#define _ImageInput

class ImageInput: public GPUImageFilter{

public:
    ImageInput();
    ImageInput(AAssetManager *assetManager,std::string path);
    virtual ~ImageInput();
    void init();
    void onInputSizeChanged(const int width, const int height);
    int onDrawFrame(const GLuint textureId, GLfloat *matrix,const float* cubeBuffer, const float* textureBuffer);
    int onDrawFrame(const GLuint textureId, GLfloat *matrix);
    GLuint onDrawToTexture(const GLuint textureId,GLfloat *matrix);
    void destroy();
    void initFrameBuffer(int width,int height);
    void destroyFrameBuffers();
    void setBeautyLevel(int level);
    GLuint imgTexture;
    int mImageWidth = -1;
    int mImageHeight = -1;
    int mImageChannel= -1;

protected:
    virtual void onInit();
    virtual void onInitialized();
    bool mIsInitialized;
    void setBeautyLevelOnDraw(int level);
    void setTexelSize(int width,int height);

private:
    std::string* mVertexShader;
    std::string* mFragmentShader;
    float* mGLCubeBuffer;
    float* mGLTextureBuffer;
    const int NO_TEXTURE = -1;
    const int NOT_INIT = -1;
    const int ON_DRAWN = 1;
    GLint mMatrixLoc;
    GLint mTexturetransformMatrixlocation;
    GLint mSingleStepOffsetLocation;
    GLint mParamsLocation;

    GLuint mFrameBuffer;
    GLuint mFrameBufferTextures;
    int beautyLevel;
    std::string imgPath;
    int degree=0;
};

#endif