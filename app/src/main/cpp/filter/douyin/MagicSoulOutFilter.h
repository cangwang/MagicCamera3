#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2019.1.4
 */
class MagicSoulOutFilter: public GPUImageFilter{

public:
    MagicSoulOutFilter();
    MagicSoulOutFilter(AAssetManager *assetManager);
    ~MagicSoulOutFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    float mProgress;
    float* mMvpMatrix;
    int mFrames=0;
    int mMaxFrames = 15;
    int mSkipFrames = 8;
    GLint mAlphaLocation;
    GLint mMvpMatrixLocation;
    float alpha;
};