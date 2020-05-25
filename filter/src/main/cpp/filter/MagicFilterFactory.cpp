#include "MagicFilterFactory.h"
#include "filter/advanced/MagicAmaroFilter.h"
#include "filter/advanced/MagicAntiqueFilter.h"
#include "filter/advanced/MagicBlackCatFilter.h"
#include "filter/advanced/MagicBrannanFilter.h"
#include "filter/advanced/MagicBrooklynFilter.h"
#include "filter/advanced/MagicCalmFilter.h"
#include "filter/advanced/MagicCoolFilter.h"
#include "filter/advanced/MagicCrayonFilter.h"
#include "filter/advanced/MagicEarlyBirdFilter.h"
#include "filter/advanced/MagicEmeraldFilter.h"
#include "filter/advanced/MagicEvergreenFilter.h"
#include "filter/advanced/MagicFreudFilter.h"
#include "filter/advanced/MagicFairytableFilter.h"
#include "filter/advanced/MagicHealthyFilter.h"
#include "filter/advanced/MagicHefeFilter.h"
#include "filter/advanced/MagicHudsonFilter.h"
#include "filter/advanced/MagicInkwellFilter.h"
#include "filter/advanced/MagicKevinFilter.h"
#include "filter/advanced/MagicLatteFilter.h"
#include "filter/advanced/MagicLomoFilter.h"
#include "filter/advanced/MagicN1977Filter.h"
#include "filter/advanced/MagicNashvilleFilter.h"
#include "filter/advanced/MagicNostalgiaFilter.h"
#include <filter/advanced/MagicNoneFilter.h>
#include "filter/advanced/MagicPixarFilter.h"
#include "filter/advanced/MagicRiseFilter.h"
#include "filter/advanced/MagicRomanceFilter.h"
#include "filter/advanced/MagicSakuraFilter.h"
#include "filter/advanced/MagicSierraFilter.h"
#include "filter/advanced/MagicSketchFilter.h"
#include "filter/advanced/MagicSkinWhiteFilter.h"
#include "filter/advanced/MagicSunriseFilter.h"
#include "filter/advanced/MagicSunsetFilter.h"
#include "filter/advanced/MagicSutroFilter.h"
#include "filter/advanced/MagicSweetsFilter.h"
#include "filter/advanced/MagicTenderFilter.h"
#include "filter/advanced/MagicToasterFilter.h"
#include "filter/advanced/MagicValenciaFilter.h"
#include "filter/advanced/MagicWaldenFilter.h"
#include "filter/advanced/MagicWarmFilter.h"
#include "filter/advanced/MagicWhiteCatFilter.h"
#include "filter/advanced/MagicXproIIFilter.h"
#include "filter/douyin/MagicSoulOutFilter.h"
#include "filter/douyin/MagicGlitchFilter.h"
#include "filter/douyin/MagicScaleFilter.h"
#include "filter/douyin/MagicShineWhiteFilter.h"
#include "filter/douyin/MagicShakeEffectFilter.h"
#include "filter/douyin/MagicVerigoFilter.h"
#include "filter/douyin/MagicThreeWinFilter.h"
#include "utils/OpenglUtils.h"

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
        case GLITCH:
            return new MagicGlitchFilter(assetManager);
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
        case SCALE:
            return new MagicScaleFilter(assetManager);
        case SAKURA:
            return new MagicSakuraFilter(assetManager);
        case SHAKE:
            return new MagicShakeEffectFilter(assetManager);
        case SHINEWHITE:
            return new MagicShineWhiteFilter(assetManager);
        case SIERRA:
            return new MagicSierraFilter(assetManager);
        case SKETCH:
            return new MagicSketchFilter(assetManager);
        case SKINWHITEN:
            return new MagicSkinWhiteFilter(assetManager);
        case SOULOUT:
            return new MagicSoulOutFilter(assetManager);
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
        case THREEWIN:
            return new MagicThreeWinFilter(assetManager);
        case TOASTER2:
            return new MagicToasterFilter(assetManager);
        case VALENCIA:
            return new MagicValenciaFilter(assetManager);
        case VERIGO:
            return new MagicVerigoFilter(assetManager);
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
            SOULOUT,
            SHAKE,
            GLITCH,
            SCALE,
            SHINEWHITE,
            VERIGO,
            THREEWIN,
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