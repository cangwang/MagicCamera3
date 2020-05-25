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
class MagicValenciaFilter: public GPUImageFilter{

public:
    MagicValenciaFilter();
    MagicValenciaFilter(AAssetManager *assetManager);
    ~MagicValenciaFilter();
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