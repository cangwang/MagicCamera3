#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/advanced/MagicLookupFilter.h"

/**
 * cangwang 2018.12.17
 */
class MagicFairytableFilter: public MagicLookupFilter{

public:
    MagicFairytableFilter();
    MagicFairytableFilter(AAssetManager *assetManager);
    ~MagicFairytableFilter();
};