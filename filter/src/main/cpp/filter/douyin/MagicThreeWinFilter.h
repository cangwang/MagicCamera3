#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang 2019.1.4
 */
class MagicThreeWinFilter: public GPUImageFilter{

public:
    MagicThreeWinFilter();
    MagicThreeWinFilter(AAssetManager *assetManager);
    ~MagicThreeWinFilter();
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
    int mMaxFrames = 14;
    int mMiddleFrames = mMaxFrames/2;
    GLint mMvpMatrixLocation;
    float alpha;
};