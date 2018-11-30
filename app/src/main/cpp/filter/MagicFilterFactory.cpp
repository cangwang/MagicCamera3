#include "MagicFilterFactory.h"
#include "src/main/cpp/filter/advanced/MagicAmaroFilter.h"
#include "src/main/cpp/filter/advanced/MagicAntiqueFilter.h"
#include "src/main/cpp/filter/advanced/MagicBlackCatFilter.h"
#include "src/main/cpp/filter/advanced/MagicBrannanFilter.h"
#include "src/main/cpp/filter/advanced/MagicBrooklynFilter.h"
#include "src/main/cpp/filter/advanced/MagicCalmFilter.h"
#include "src/main/cpp/filter/advanced/MagicCoolFilter.h"
#include "src/main/cpp/filter/advanced/MagicCrayonFilter.h"
#include "src/main/cpp/filter/advanced/MagicEarlyBirdFilter.h"
#include "src/main/cpp/filter/advanced/MagicEmeraldFilter.h"
#include "src/main/cpp/filter/advanced/MagicEvergreenFilter.h"
#include "src/main/cpp/filter/advanced/MagicFreudFilter.h"
#include "src/main/cpp/filter/advanced/MagicHealthyFilter.h"
#include "src/main/cpp/filter/advanced/MagicHefeFilter.h"
#include "src/main/cpp/filter/advanced/MagicInkwellFilter.h"
#include "src/main/cpp/filter/advanced/MagicKevinFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicFilterFactory"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

GPUImageFilter* initFilters(MagicFilterType type,AAssetManager* assetManager){
    switch (type){
        case AMARO:
            return new MagicAmaroFilter(assetManager);
        case ANTIQUE:
            return new MagicAmaroFilter(assetManager);
        case BLACKCAT:
            return new MagicBlackCatFilter(assetManager);
        case BRANNAN:
            return new MagicBrannanFilter(assetManager);
        case BROOKLYN:
            return new MagicBrooklynFilter(assetManager);
        case CALM:
            return new MagicCalmFilter(assetManager);
        case COOL:
            return new MagicCoolFilter(assetManager);
        default:
            return nullptr;
    }
}

int* getFilterTypes(){
    static int types[]={NONE,AMARO,ANTIQUE,BRANNAN,BROOKLYN,CALM,COOL};
    return types;
}