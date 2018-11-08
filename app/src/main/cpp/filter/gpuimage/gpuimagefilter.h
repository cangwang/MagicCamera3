#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


class GPUImageFilter {

public:
    virtual ~GPUImageFilter();
    void init();
    void onInputSizeChanged(const int width, const int height);
    int onDrawFrame(const GLint textureId, const float* cubeBuffer, const float* textureBuffer);
    int onDrawFrame(const GLint textureId);

protected:
    GPUImageFilter();
    GPUImageFilter(char* vertexShader,char* fragmentShader);
    void onInit();
    void onInitialized();
    bool mIsInitialized;
    GLuint mGLProgId;
    GLint mGLAttribPosition;
    GLint mGLUniformTexture;
    GLint mGLAttribTexureCoordinate;
    int mInputWidth;
    int mInputHeight;
    void onDrawArraysPre(){}
    void onDrawArraysAffter(){}

private:
    char* mVertexShader;
    char* mFragmentShader;
    float mGLCubeBuffer[];
    float mGLTextureBuffer[];
};