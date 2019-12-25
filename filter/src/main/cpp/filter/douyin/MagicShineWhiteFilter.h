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
class MagicShineWhiteFilter: public GPUImageFilter{

public:
    MagicShineWhiteFilter();
    MagicShineWhiteFilter(AAssetManager *assetManager);
    ~MagicShineWhiteFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    float mProgress;
    int mFrames=0;
    int mMaxFrames = 14;
    int mMiddleFrames = mMaxFrames/2;
    GLint mAdditionColorLocation;
    float alpha;
};