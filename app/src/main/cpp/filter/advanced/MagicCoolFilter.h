#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

class MagicCoolFilter: public GPUImageFilter{

public:
    MagicCoolFilter();
    MagicCoolFilter(AAssetManager *assetManager);
    ~MagicCoolFilter();
    void onDestroy() override ;

protected:
    void onInit();
    void onInitialized();
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint mToneCurveTexture = static_cast<GLuint>(-1);
    GLint mToneCurveTextureUniformLocation;
};