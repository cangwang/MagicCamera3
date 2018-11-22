#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

class MagicCalmFilter: public GPUImageFilter{

public:
    MagicCalmFilter();
    MagicCalmFilter(AAssetManager *assetManager);
    ~MagicCalmFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLint mToneCurveTexture;
    GLint mToneCurveTextureUniformLocation;
    GLint mMaskGery1TextureId;
    GLint mMaskGery1UniformLocation;
    GLint mMaskGery2TextureId;
    GLint mMaskGery2UniformLocation;
    AAssetManager* assetManager;
};