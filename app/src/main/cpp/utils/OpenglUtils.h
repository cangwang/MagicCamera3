#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/bitmap/BitmapOperation.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class OpenGlUtils{
public:
    static const int NO_TEXTURE = -1;
    static const int NOT_INIT = -1;
    static const int ON_DRAWN = 1;
    static int loadTexture(const char *img,const int userTexId);
    static int loadTexture(const char *img,const int userTexId, bool recyled);
    static int loadTexture(const char data[],const int width,const int height,const int userTexId);
    static int loadTexture(const char data[],const int width,const int height,const int userTexId,int type);
    static int loadTexture(const JNIEnv *env,const char* name);
    static BitmapOperation getImageFromAssetsFile(JNIEnv *env,char *filename);
    static GLuint loadProgram(const char *strVSource, const char *strFSource);
    static GLuint loadShader(const char *strSource,const int iType);
    static GLuint getExternalOESTextureID();
    static char* readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager, char* fileName);
    static BitmapOperation drawToBitmapByFilter(jobject bitmap,GPUImageFilter filter,int displayWidth,int displayHeight, bool rotate);
    static void checkGLError(char *op);
    static void freeResource(char *pData);
};

