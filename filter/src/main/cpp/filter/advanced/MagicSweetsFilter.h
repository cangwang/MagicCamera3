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
class MagicSweetsFilter: public GPUImageFilter{

public:
    MagicSweetsFilter();
    MagicSweetsFilter(AAssetManager *assetManager);
    ~MagicSweetsFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint mToneCurveTexture;
    GLint mToneCurveTextureUniformLocation;
    GLuint mMaskGery1TextureId;
    GLint mMaskGery1UniformLocation;
    GLint mLowPerformanceUniformLocation;
};