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
class MagicSketchFilter: public GPUImageFilter{

public:
    MagicSketchFilter();
    MagicSketchFilter(AAssetManager *assetManager);
    ~MagicSketchFilter();
    void onInputSizeChanged(const int width, const int height) override ;
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLint mGLStrengthLocation;
    GLint mSingleStepOffsetLocation;
    GLfloat mChangeWidth;
    GLfloat mChangeHeight;
};