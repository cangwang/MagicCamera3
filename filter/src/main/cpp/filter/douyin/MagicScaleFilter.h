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
class MagicScaleFilter: public GPUImageFilter{

public:
    MagicScaleFilter();
    MagicScaleFilter(AAssetManager *assetManager);
    ~MagicScaleFilter();
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
    GLint mAlphaLocation;
    GLint mMvpMatrixLocation;
    float alpha;
};