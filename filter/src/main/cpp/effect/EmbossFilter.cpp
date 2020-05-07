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

#include "EmbossFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(EmbossFilter)

EmbossFilter* EmbossFilter::create() {
    EmbossFilter* ret = new (std::nothrow) EmbossFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool EmbossFilter::init() {
    if (!Convolution3x3Filter::init()) return false;

    _intensity = 1.0;
    setIntensity(_intensity);
    
    registerProperty("intensity", _intensity, "The strength of the embossing, from  0.0 to 4.0, with 1.0 as the normal level", [this](float& intensity){
        setIntensity(intensity);
    });

    return true;
}

void EmbossFilter::setIntensity(float intensity) {
    _intensity = intensity;
    if (_intensity > 4.0) _intensity = 4.0;
    else if (_intensity < 0.0) _intensity = 0.0;
    
    _convolutionKernel.set(-2.0 * _intensity, -_intensity, 0.0,
                           -_intensity, 1.0, _intensity,
                           0.0, _intensity, _intensity * 2.0
                           );
}


