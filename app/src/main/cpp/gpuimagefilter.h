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

protected:
    GPUImageFilter();
    GPUImageFilter(char* vertexShader,char* fragmentShader);
    void onInit();
    void onInitialized();

private:
    char* vertexShader;
    char* fragmentShader;

};