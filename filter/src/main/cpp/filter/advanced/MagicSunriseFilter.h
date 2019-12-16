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
class MagicSunriseFilter: public GPUImageFilter{

public:
    MagicSunriseFilter();
    MagicSunriseFilter(AAssetManager *assetManager);
    ~MagicSunriseFilter();
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
    GLuint mMaskGery2TextureId;
    GLint mMaskGery2UniformLocation;
    GLuint mMaskGery3TextureId;
    GLint mMaskGery3UniformLocation;
};