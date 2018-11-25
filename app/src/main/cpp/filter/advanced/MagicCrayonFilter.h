#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

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
};