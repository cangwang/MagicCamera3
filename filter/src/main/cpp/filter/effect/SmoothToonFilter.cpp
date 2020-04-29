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

#include "SmoothToonFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(SmoothToonFilter)

SmoothToonFilter::SmoothToonFilter()
:_gaussianBlurFilter(0)
,_toonFilter(0)
{
}

SmoothToonFilter::~SmoothToonFilter() {
    if (_gaussianBlurFilter) {
        _gaussianBlurFilter->release();
        _gaussianBlurFilter = 0;
    }
    
    if (_toonFilter) {
        _toonFilter->release();
        _toonFilter = 0;
    }
    
}

SmoothToonFilter* SmoothToonFilter::create() {
    SmoothToonFilter* ret = new (std::nothrow) SmoothToonFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool SmoothToonFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }
    _gaussianBlurFilter = GaussianBlurFilter::create();
    _toonFilter = ToonFilter::create();
    _gaussianBlurFilter->addTarget(_toonFilter);
    addFilter(_gaussianBlurFilter);
    
    
    _blurRadius = 2.0;
    setBlurRadius(_blurRadius);
    registerProperty("blurRadius", _blurRadius, "", [this](int& blurRadius){
        setBlurRadius(blurRadius);
    });
    
    _toonThreshold = 0.2;
    registerProperty("toonThreshold", _toonThreshold, "The threshold at which to apply the edges", [this](float& toonThreshold){
        setToonThreshold(toonThreshold);
    });
    
    _toonQuantizationLevels = 10.0;
    registerProperty("toonQuantizationLevels", _toonQuantizationLevels, "The levels of quantization for the posterization of colors within the scene", [this](float& toonQuantizationLevels){
        setToonQuantizationLevels(toonQuantizationLevels);
    });
    
    
    return true;
}

void SmoothToonFilter::setBlurRadius(int blurRadius) {
    _blurRadius = blurRadius;
    _gaussianBlurFilter->setRadius(_blurRadius);
}

void SmoothToonFilter::setToonThreshold(float toonThreshold) {
    _toonThreshold = toonThreshold;
    _toonFilter->setThreshold(_toonThreshold);
}

void SmoothToonFilter::setToonQuantizationLevels(float toonQuantizationLevels) {
    _toonQuantizationLevels = toonQuantizationLevels;
    _toonFilter->setQuantizatinLevels(_toonQuantizationLevels);
}


NS_GI_END
