#include "MagicFairytableFilter.h"
#include "utils/OpenglUtils.h"

#define LOG_TAG "MagicFairytableFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define GET_ARRAY_LEN(array,len){len = (sizeof(array) / sizeof(array[0]));}

/**
 * cangwang 2018.12.17
 */
MagicFairytableFilter::MagicFairytableFilter(){

}

MagicFairytableFilter::MagicFairytableFilter(AAssetManager *assetManager)
    : MagicLookupFilter(assetManager,"fairy_tale.png"){

}

MagicFairytableFilter::~MagicFairytableFilter() {

}
