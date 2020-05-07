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

#include "LuminanceRangeFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(LuminanceRangeFilter)

const std::string kLuminanceRangeFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform lowp float rangeReductionFactor;
    varying highp vec2 vTexCoord;
 
    // Values from "Graphics Shaders: Theory and Practice" by Bailey and Cunningham
    const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        mediump float luminance = dot(color.rgb, luminanceWeighting);
        mediump float luminanceRatio = ((0.5 - luminance) * rangeReductionFactor);
        gl_FragColor = vec4((color.rgb) + (luminanceRatio), color.a);
    }
);

LuminanceRangeFilter* LuminanceRangeFilter::create() {
    LuminanceRangeFilter* ret = new (std::nothrow) LuminanceRangeFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool LuminanceRangeFilter::init() {
    if (!initWithFragmentShaderString(kLuminanceRangeFragmentShaderString)) return false;

    _rangeReductionFactor = 0.6;
    registerProperty("rangeReductionFactor", _rangeReductionFactor, "The degree to reduce the luminance range, from 0.0 to 1.0. Default is 0.6.", [this](float& rangeReductionFactor){
        setRangeReductionFactor(rangeReductionFactor);
    });

    return true;
}

void LuminanceRangeFilter::setRangeReductionFactor(float rangeReductionFactor) {
    _rangeReductionFactor = rangeReductionFactor;
    if (_rangeReductionFactor > 1.0) _rangeReductionFactor = 1.0;
    else if (_rangeReductionFactor < 0.0) _rangeReductionFactor = 0.0;
}

bool LuminanceRangeFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("rangeReductionFactor", _rangeReductionFactor);
    return Filter::proceed(bUpdateTargets);
}

