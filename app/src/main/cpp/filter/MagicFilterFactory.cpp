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
#include "src/main/cpp/filter/advanced/MagicFairytableFilter.h"
#include "src/main/cpp/filter/advanced/MagicHealthyFilter.h"
#include "src/main/cpp/filter/advanced/MagicHefeFilter.h"
#include "src/main/cpp/filter/advanced/MagicHudsonFilter.h"
#include "src/main/cpp/filter/advanced/MagicInkwellFilter.h"
#include "src/main/cpp/filter/advanced/MagicKevinFilter.h"
#include "src/main/cpp/filter/advanced/MagicLatteFilter.h"
#include "src/main/cpp/filter/advanced/MagicLomoFilter.h"
#include "src/main/cpp/filter/advanced/MagicN1977Filter.h"
#include "src/main/cpp/filter/advanced/MagicNashvilleFilter.h"
#include "src/main/cpp/filter/advanced/MagicNostalgiaFilter.h"
#include <src/main/cpp/filter/advanced/MagicNoneFilter.h>
#include "src/main/cpp/filter/advanced/MagicPixarFilter.h"
#include "src/main/cpp/filter/advanced/MagicRiseFilter.h"
#include "src/main/cpp/filter/advanced/MagicRomanceFilter.h"
#include "src/main/cpp/filter/advanced/MagicSakuraFilter.h"
#include "src/main/cpp/filter/advanced/MagicSierraFilter.h"
#include "src/main/cpp/filter/advanced/MagicSketchFilter.h"
#include "src/main/cpp/filter/advanced/MagicSkinWhiteFilter.h"
#include "src/main/cpp/filter/advanced/MagicSunriseFilter.h"
#include "src/main/cpp/filter/advanced/MagicSunsetFilter.h"
#include "src/main/cpp/filter/advanced/MagicSutroFilter.h"
#include "src/main/cpp/filter/advanced/MagicSweetsFilter.h"
#include "src/main/cpp/filter/advanced/MagicTenderFilter.h"
#include "src/main/cpp/filter/advanced/MagicToasterFilter.h"
#include "src/main/cpp/filter/advanced/MagicValenciaFilter.h"
#include "src/main/cpp/filter/advanced/MagicWaldenFilter.h"
#include "src/main/cpp/filter/advanced/MagicWarmFilter.h"
#include "src/main/cpp/filter/advanced/MagicWhiteCatFilter.h"
#include "src/main/cpp/filter/advanced/MagicXproIIFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "MagicFilterFactory"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

/**
 * cangwang 2018.12.1
 */

GPUImageFilter* initFilters(int type,AAssetManager* assetManager){
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
        case CRAYON:
            return new MagicCrayonFilter(assetManager);
        case EARLYBIRD:
            return new MagicEarlyBirdFilter(assetManager);
        case EMERALD:
            return new MagicEmeraldFilter(assetManager);
        case EVERGREEN:
            return new MagicEvergreenFilter(assetManager);
        case FREUD:
            return new MagicFreudFilter(assetManager);
        case FAIRYTALE:
            return new MagicFairytableFilter(assetManager);
        case HEALTHY:
            return new MagicHealthyFilter(assetManager);
        case HEFE:
            return new MagicHefeFilter(assetManager);
        case HUDSON:
            return new MagicHudsonFilter(assetManager);
        case INKWELL:
            return new MagicInkwellFilter(assetManager);
        case KEVIN:
            return new MagicKevinFilter(assetManager);
        case LATTE:
            return new MagicLatteFilter(assetManager);
        case LOMO:
            return new MagicLomoFilter(assetManager);
        case N1977:
            return new MagicN1977Filter(assetManager);
        case NOSTALGIA:
            return new MagicNostalgiaFilter(assetManager);
        case NASHVILLE:
            return new MagicNashvilleFilter(assetManager);
        case NONE:
            return new MagicNoneFilter(assetManager);
        case PIXAR:
            return new MagicPixarFilter(assetManager);
        case RISE:
            return new MagicRiseFilter(assetManager);
        case ROMANCE:
            return new MagicRomanceFilter(assetManager);
        case SAKURA:
            return new MagicSakuraFilter(assetManager);
        case SIERRA:
            return new MagicSierraFilter(assetManager);
        case SKETCH:
            return new MagicSketchFilter(assetManager);
        case SKINWHITEN:
            return new MagicSkinWhiteFilter(assetManager);
        case SUNRISE:
            return new MagicSunriseFilter(assetManager);
        case SUNSET:
            return new MagicSunsetFilter(assetManager);
        case SUTRO:
            return new MagicSutroFilter(assetManager);
        case SWEETS:
            return new MagicSweetsFilter(assetManager);
        case TENDER:
            return new MagicTenderFilter(assetManager);
        case TOASTER2:
            return new MagicToasterFilter(assetManager);
        case VALENCIA:
            return new MagicValenciaFilter(assetManager);
        case WALDEN:
            return new MagicWaldenFilter(assetManager);
        case WARM:
            return new MagicWarmFilter(assetManager);
        case WHITECAT:
            return new MagicWhiteCatFilter(assetManager);
        case XPROII:
            return new MagicXproIIFilter(assetManager);
        default:
            return new GPUImageFilter(assetManager);
    }
}

int* getFilterTypes(int &len){
    static int types[]={
            NONE,
            FAIRYTALE,
            SUNRISE,
            SUNSET,
            WHITECAT,
            BLACKCAT,
            SKINWHITEN,
            HEALTHY,
            SWEETS,
            ROMANCE,
            SAKURA,
            WARM,
            ANTIQUE,
            NOSTALGIA,
            CALM,
            LATTE,
            TENDER,
            COOL,
            EMERALD,
            EVERGREEN,
            CRAYON,
            SKETCH,
            AMARO,
            BRANNAN,
            BROOKLYN,
            EARLYBIRD,
            FREUD,
            HEFE,
            HUDSON,
            INKWELL,
            KEVIN,
            LOMO,
            N1977,
            NASHVILLE,
            PIXAR,
            RISE,
            SIERRA,
            SUTRO,
            TOASTER2,
            VALENCIA,
            WALDEN,
            XPROII,
            //image adjust
            CONTRAST,
            BRIGHTNESS,
            EXPOSURE,
            HUE,
            SATURATION,
            SHARPEN,
            IMAGE_ADJUST};
    len = sizeof(types)/ sizeof(types[0]) - 7;
    return types;
}