#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

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