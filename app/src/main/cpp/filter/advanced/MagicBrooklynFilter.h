#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

class MagicBrooklynFilter: public GPUImageFilter{

public:
    MagicBrooklynFilter();
    MagicBrooklynFilter(AAssetManager *assetManager);
    ~MagicBrooklynFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint inputTextureHandles[3]={0};
    GLint inputTextureUniformLocations[3]={0};
    int len;
    int mGLStrengthLocation;
};