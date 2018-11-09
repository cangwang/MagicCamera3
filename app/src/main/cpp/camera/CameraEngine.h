#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/bitmap/BitmapOperation.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "CameraEngine"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class CameraEngine{
public:
    GPUImageFilter filter;
    CameraEngine();
    ~CameraEngine();
    void create();
    void changed(int width,int height);
    void draw();

protected:
    GLint textureId;
    float glCubeBuffer;
    float glTextureBuffer;
    int surfaceWidth;
    int surfaceHeight;
    int imageWidth;
    int imageHeight;
};

