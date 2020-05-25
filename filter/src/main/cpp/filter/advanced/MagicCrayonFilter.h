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
class MagicCrayonFilter: public GPUImageFilter{

public:
    MagicCrayonFilter();
    MagicCrayonFilter(AAssetManager *assetManager);
    ~MagicCrayonFilter();
    void onInputSizeChanged(const int width, const int height) override ;
    void onDestroy() override ;
    void setTexelSize(const float w, const float h);

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    GLint mSingleStepOffsetLocation;
    GLint mStrengthLocation;
    GLfloat mChangeWidth;
    GLfloat mChangeHeight;
};