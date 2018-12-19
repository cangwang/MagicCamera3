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
class MagicBeautyFilter: public GPUImageFilter{

public:
    MagicBeautyFilter();
    MagicBeautyFilter(AAssetManager *assetManager);
    ~MagicBeautyFilter();
    void onInputSizeChanged(const int width, const int height) override ;
    void onDestroy() override ;
    void setTexelSize(const float w, const float h);

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;
    void setBeautyLevel(int level);

private:
    GLint mSingleStepOffsetLocation;
    GLint mParamsLocation;
    GLfloat mChangeWidth;
    GLfloat mChangeHeight;
};