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
class MagicSkinWhiteFilter: public GPUImageFilter{

public:
    MagicSkinWhiteFilter();
    MagicSkinWhiteFilter(AAssetManager *assetManager);
    ~MagicSkinWhiteFilter();
    void onInputSizeChanged(const int width, const int height) override ;
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint mToneCurveTexture;
    GLint mToneCurveTextureUniformLocation;
    GLint mTexelHeightUniformLocation;
    GLint mTexelWidthUniformLocation;
    GLfloat mChangeWidth;
    GLfloat mChangeHeight;
};