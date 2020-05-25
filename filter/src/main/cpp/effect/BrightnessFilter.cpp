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

#include "BrightnessFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(BrightnessFilter)

const std::string kBrightnessFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform lowp float brightness;
    varying highp vec2 vTexCoord;
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        gl_FragColor = vec4((color.rgb + vec3(brightness)), color.a);
    }
);


BrightnessFilter* BrightnessFilter::create(float brightness/* = 0.0*/) {
    BrightnessFilter* ret = new (std::nothrow) BrightnessFilter();
    if (ret && !ret->init(brightness)) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool BrightnessFilter::init(float brightness) {
    if (!initWithFragmentShaderString(kBrightnessFragmentShaderString)) return false;

    _brightness = brightness;
    registerProperty("brightness", _brightness, "The brightness of filter with range between -1 and 1.", [this](float& brightness){
        setBrightness(brightness);
    });

    return true;
}

void BrightnessFilter::setBrightness(float brightness) {
    _brightness = brightness;
    if (_brightness > 1.0) _brightness = 1.0;
    else if (_brightness < -1.0) _brightness = -1.0;
}

bool BrightnessFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("brightness", _brightness);
    return Filter::proceed(bUpdateTargets);
}

