#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>


class GPUImageFilter {

public:
    GPUImageFilter();
    GPUImageFilter(char* vertexShader,char* fragmentShader);
    virtual ~GPUImageFilter();
    void init();
    void onInputSizeChanged(const int width, const int height);
    int onDrawFrame(const GLint textureId, const float* cubeBuffer, const float* textureBuffer);
    int onDrawFrame(const GLint textureId);

protected:
    void onInit();
    void onInitialized();
    bool mIsInitialized;
    GLuint mGLProgId;
    GLint mGLAttribPosition;
    GLint mGLUniformTexture;
    GLint mGLAttribTexureCoordinate;
    int mInputWidth;
    int mInputHeight;
    virtual void onDrawArraysPre();
    virtual void onDrawArraysAfter();


private:
    char* mVertexShader;
    char* mFragmentShader;
    float* mGLCubeBuffer;
    float* mGLTextureBuffer;
};