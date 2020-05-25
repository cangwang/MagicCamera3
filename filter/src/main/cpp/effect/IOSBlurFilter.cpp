/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "IOSBlurFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(IOSBlurFilter)

IOSBlurFilter::IOSBlurFilter()
:_saturationFilter(0)
,_blurFilter(0)
,_luminanceRangeFilter(0)
{
}

IOSBlurFilter::~IOSBlurFilter() {
    if (_saturationFilter) {
        _saturationFilter->release();
        _saturationFilter = 0;
    }
    
    if (_blurFilter) {
        _blurFilter->release();
        _blurFilter = 0;
    }
    
    if (_luminanceRangeFilter) {
        _luminanceRangeFilter->release();
        _luminanceRangeFilter = 0;
    }
}

IOSBlurFilter* IOSBlurFilter::create() {
    IOSBlurFilter* ret = new (std::nothrow) IOSBlurFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool IOSBlurFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }
    
    _saturationFilter = SaturationFilter::create();
    _blurFilter = GaussianBlurFilter::create();
    _luminanceRangeFilter = LuminanceRangeFilter::create();
    
    _saturationFilter                   // 1. downsample and desaturate
    ->addTarget(_blurFilter)            // 2. apply a strong Gaussian blur
    ->addTarget(_luminanceRangeFilter); // 3. upsample and adjust luminance range
    
    addFilter(_saturationFilter);
    
    _blurSigma = 12.0;
    setBlurSigma(_blurSigma);
    registerProperty("blurSigma", _blurSigma, "", [this](float& blurSigma){
        setBlurSigma(blurSigma);
    });
    
    _saturation = 0.8;
    setSaturation(_saturation);
    registerProperty("saturation", _saturation, "", [this](float& saturation){
        setSaturation(saturation);
    });
    
    _rangeReductionFactor = 0.6;
    setRangeReductionFactor(_rangeReductionFactor);
    registerProperty("rangeReductionFactor", _rangeReductionFactor, "", [this](float& rangeReductionFactor){
        setRangeReductionFactor(rangeReductionFactor);
    });
    
    _downSampling = 4.0;
    setDownSampling(_downSampling);
    registerProperty("downSampling", _downSampling, "", [this](float& downSampling){
        setDownSampling(downSampling);
    });

    return true;
}


void IOSBlurFilter::setBlurSigma(float blurSigma) {
    _blurSigma = blurSigma;
    _blurFilter->setSigma(blurSigma);
}

void IOSBlurFilter::setSaturation(float saturation) {
    _saturation = saturation;
    _saturationFilter->setSaturation(saturation);
}

void IOSBlurFilter::setRangeReductionFactor(float rangeReductionFactor) {
    _rangeReductionFactor = rangeReductionFactor;
    _luminanceRangeFilter->setRangeReductionFactor(rangeReductionFactor);
}

void IOSBlurFilter::setDownSampling(float downSampling) {
    _downSampling = downSampling;
    _saturationFilter->setFramebufferScale(1 / downSampling);
    _luminanceRangeFilter->setFramebufferScale(downSampling);
}

NS_GI_END
