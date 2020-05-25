#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2018.12.1
 */
class MagicAntiqueFilter: public GPUImageFilter{

public:
    MagicAntiqueFilter();
    MagicAntiqueFilter(AAssetManager *assetManager);
    ~MagicAntiqueFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLuint mToneCurveTexture;
    GLint mToneCurveTextureUniformLocation;
};