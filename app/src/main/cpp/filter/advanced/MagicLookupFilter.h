#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2018.12.17
 */
class MagicLookupFilter: public GPUImageFilter{

public:
    MagicLookupFilter();
    MagicLookupFilter(AAssetManager *assetManager,std::string);
    ~MagicLookupFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint inputTextureHandles;
    GLint inputTextureUniformLocations;
    int mGLStrengthLocation;
    std::string table;
};