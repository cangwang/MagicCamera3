#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

#define LOG_TAG "MagicCoolFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


class MagicCoolFilter: protected GPUImageFilter{

public:
    MagicCoolFilter();
    ~MagicCoolFilter();
    void onDestroy();

protected:
    void onInit();
    void onInitialized();
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint mToneCurveTexture[]={-1};
    GLint mToneCurveTextureUniformLocation;
};