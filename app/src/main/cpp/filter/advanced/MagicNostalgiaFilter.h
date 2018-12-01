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
class MagicNostalgiaFilter: public GPUImageFilter{

public:
    MagicNostalgiaFilter();
    MagicNostalgiaFilter(AAssetManager *assetManager);
    ~MagicNostalgiaFilter();
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
    GLuint mToneCurveTexture2;
    GLint mToneCurveTextureUniformLocation2;
    GLint mBlurSizeUniformLocation;
    GLint mTexelHeightUniformLocation;
    GLint mTexelWidthUniformLocation;

};