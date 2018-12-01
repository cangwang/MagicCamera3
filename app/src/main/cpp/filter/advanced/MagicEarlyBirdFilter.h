#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2018.12.1
 */
class MagicEarlyBirdFilter: public GPUImageFilter{

public:
    MagicEarlyBirdFilter();
    MagicEarlyBirdFilter(AAssetManager *assetManager);
    ~MagicEarlyBirdFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint inputTextureHandles[5]={0};
    GLint inputTextureUniformLocations[5]={0};
    int len;
    int mGLStrengthLocation;
};