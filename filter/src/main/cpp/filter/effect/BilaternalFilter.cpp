#include "BilaternalFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(BilateralMonoFilter)

BilateralMonoFilter::BilateralMonoFilter(GPUImage::BilateralMonoFilter::Type type)
:_type(type)
,_texelSpcingMultiplier(4.0)
,_distanceNormalizationFactor(8.0){

}

BilateralMonoFilter* BilateralMonoFilter::create(GPUImage::BilateralMonoFilter::Type type) {
    auto ret = new (std::nothrow) BilateralMonoFilter(type);
    if (!ret || !ret->init()) {
        delete ret;
        ret = nullptr;
    }
    return ret;
}



NS_GI_END