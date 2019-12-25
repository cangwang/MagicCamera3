#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "filter/gpuimage/GpuImageFilter.h"

/**
 * cangwang  2019.1.4
 */
class MagicGlitchFilter: public GPUImageFilter{

public:
    MagicGlitchFilter();
    MagicGlitchFilter(AAssetManager *assetManager);
    ~MagicGlitchFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter() override;

private:
    float* mDriftSequence;
    float* mJitterSequence;
    float* mThreshHoldSequence;
    int mFrames=0;
    int mMaxFrames = 8;
    GLint mScanLineJitterLocation;
    GLint mColorDriftLocation;
    float alpha;
};