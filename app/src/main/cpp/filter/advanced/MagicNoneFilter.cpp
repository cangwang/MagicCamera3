#include "MagicNoneFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicNoneFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.1
 */
MagicNoneFilter::MagicNoneFilter(){

}

MagicNoneFilter::MagicNoneFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"), readShaderFromAsset(assetManager,"none.glsl")){

}

MagicNoneFilter::~MagicNoneFilter() {

}

void MagicNoneFilter::onDestroy() {

}

void MagicNoneFilter::onDrawArraysPre() {

}

void MagicNoneFilter::onDrawArraysAfter() {

}


void MagicNoneFilter::onInit() {
    GPUImageFilter::onInit();

}

void MagicNoneFilter::onInitialized() {
    GPUImageFilter::onInitialized();
}