#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

class MagicHealthyFilter: public GPUImageFilter{

public:
    MagicHealthyFilter();
    MagicHealthyFilter(AAssetManager *assetManager);
    ~MagicHealthyFilter();
    void onDestroy() override ;
    void onInputSizeChanged(const int width, const int height) override ;

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

    GLint mTexelHeightUniformLocation;
    GLint mTexelWidthUniformLocation;

};