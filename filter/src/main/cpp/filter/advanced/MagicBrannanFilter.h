#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2018.12.1
 */
class MagicBrannanFilter: public GPUImageFilter{

public:
    MagicBrannanFilter();
    MagicBrannanFilter(AAssetManager *assetManager);
    ~MagicBrannanFilter();
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