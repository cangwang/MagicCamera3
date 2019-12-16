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
class MagicN1977Filter: public GPUImageFilter{

public:
    MagicN1977Filter();
    MagicN1977Filter(AAssetManager *assetManager);
    ~MagicN1977Filter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint inputTextureHandles[2]={0};
    GLint inputTextureUniformLocations[2]={0};
    int len;
    int mGLStrengthLocation;
};