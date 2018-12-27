#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <string>
#include "src/main/cpp/utils/TextureRotationUtil.h"

#ifndef _GPUImageFilter
#define _GPUImageFilter

class GPUImageFilter {

public:
    GPUImageFilter();
    GPUImageFilter(AAssetManager *assetManager);
    GPUImageFilter(AAssetManager *assetManager,std::string *vertexShader, std::string *fragmentShader);
    virtual ~GPUImageFilter();
    virtual void init();
    virtual void onInputSizeChanged(const int width, const int height);
    virtual int onDrawFrame(const GLuint textureId, GLfloat *matrix,const float* cubeBuffer, const float* textureBuffer);
    virtual int onDrawFrame(const GLuint textureId, GLfloat *matrix);
    virtual void destroy();
    virtual void onDestroy() {

    }

    virtual void onDrawArraysPre() {

    }

    virtual void onDrawArraysAfter() {

    }

    bool savePhoto(const GLuint textureId,std::string directory);

    AAssetManager* mAssetManager;
    int mInputWidth;
    int mInputHeight;

protected:
    virtual void onInit();
    virtual void onInitialized();
    bool mIsInitialized;
    GLuint mGLProgId;
    GLint mGLAttribPosition;
    GLint mGLUniformTexture;
    GLint mGLAttribTextureCoordinate;

private:
    std::string* mVertexShader;
    std::string* mFragmentShader;
    GLfloat* mGLCubeBuffer;
    GLfloat* mGLTextureBuffer;
    const int NO_TEXTURE = -1;
    const int NOT_INIT = -1;
    const int ON_DRAWN = 1;
    GLint mMatrixLoc;
    GLuint mFrameBuffer;
};

#endif