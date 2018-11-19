#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include <android/asset_manager.h>
#include <string>

#ifndef _GPUImageFilter
#define _GPUImageFilter

class GPUImageFilter {

public:
    GPUImageFilter();
    GPUImageFilter(AAssetManager *assetManager);
    GPUImageFilter(std::string *vertexShader, std::string *fragmentShader);
    virtual ~GPUImageFilter();
    void init();
    void onInputSizeChanged(const int width, const int height);
    int onDrawFrame(const GLint textureId, const float* cubeBuffer, const float* textureBuffer);
    int onDrawFrame(const GLint textureId);
    void destroy();
    virtual void onDestroy() {

    }

    virtual void onDrawArraysPre() {

    }

    virtual void onDrawArraysAfter() {

    }

protected:
    virtual void onInit();
    virtual void onInitialized();
    bool mIsInitialized;
    GLuint mGLProgId;
    GLint mGLAttribPosition;
    GLint mGLUniformTexture;
    GLint mGLAttribTexureCoordinate;
    int mInputWidth;
    int mInputHeight;

private:
    std::string* mVertexShader;
    std::string* mFragmentShader;
    float* mGLCubeBuffer;
    float* mGLTextureBuffer;
    const int NO_TEXTURE = -1;
    const int NOT_INIT = -1;
    const int ON_DRAWN = 1;
};

#endif