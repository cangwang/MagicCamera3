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

#include <cmath>
#include "SingleComponentGaussianBlurFilter.hpp"
#include "../source/util.h"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(SingleComponentGaussianBlurFilter)

SingleComponentGaussianBlurFilter::SingleComponentGaussianBlurFilter()
:_hBlurFilter(0)
,_vBlurFilter(0)
{
}

SingleComponentGaussianBlurFilter::~SingleComponentGaussianBlurFilter() {
    if (_hBlurFilter) {
        _hBlurFilter->release();
        _hBlurFilter = 0;
    }
    
    if (_vBlurFilter) {
        _vBlurFilter->release();
        _vBlurFilter = 0;
    }
    
}

SingleComponentGaussianBlurFilter* SingleComponentGaussianBlurFilter::create(int radius/* = 4*/, float sigma/* = 2.0*/) {
    SingleComponentGaussianBlurFilter* ret = new (std::nothrow) SingleComponentGaussianBlurFilter();
    if (ret && !ret->init(radius, sigma)) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool SingleComponentGaussianBlurFilter::init(int radius, float sigma) {
    if (!FilterGroup::init()) {
        return false;
    }
    
    _hBlurFilter = SingleComponentGaussianBlurMonoFilter::create(SingleComponentGaussianBlurMonoFilter::HORIZONTAL, radius, sigma);
    _vBlurFilter = SingleComponentGaussianBlurMonoFilter::create(SingleComponentGaussianBlurMonoFilter::VERTICAL, radius, sigma);
    _hBlurFilter->addTarget(_vBlurFilter);
    addFilter(_hBlurFilter);
    
    registerProperty("radius", 4, "", [this](int& radius){
        setRadius(radius);
    });
    
    registerProperty("sigma", 2.0, "", [this](float& sigma){
        setSigma(sigma);
    });
    
    return true;
}

void SingleComponentGaussianBlurFilter::setRadius(int radius) {
    _hBlurFilter->setRadius(radius);
    _vBlurFilter->setRadius(radius);
}

void SingleComponentGaussianBlurFilter::setSigma(float sigma) {
    _hBlurFilter->setSigma(sigma);
    _vBlurFilter->setSigma(sigma);
}

NS_GI_END
