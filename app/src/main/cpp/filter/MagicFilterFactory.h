#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"

enum MagicFilterType{
    NONE = 0,
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
    IMAGE_ADJUST
};

int* getFilterTypes();

//MagicFilterType filterType = NONE;

GPUImageFilter* initFilters(MagicFilterType type,AAssetManager* assetManager);