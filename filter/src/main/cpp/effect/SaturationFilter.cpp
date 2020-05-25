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

#include "SaturationFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(SaturationFilter)

const std::string kSaturationFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform lowp float saturation;
    varying highp vec2 vTexCoord;
 
    // Values from "Graphics Shaders: Theory and Practice" by Bailey and Cunningham
    const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        lowp float luminance = dot(color.rgb, luminanceWeighting);
        lowp vec3 greyScaleColor = vec3(luminance);

        gl_FragColor = vec4(mix(greyScaleColor, color.rgb, saturation), color.a);
    }
);


SaturationFilter* SaturationFilter::create() {
    SaturationFilter* ret = new (std::nothrow) SaturationFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool SaturationFilter::init() {
    if (!initWithFragmentShaderString(kSaturationFragmentShaderString)) return false;

    _saturation = 1.0;
    registerProperty("saturation", _saturation, "The saturation of an image. Saturation ranges from 0.0 (fully desaturated) to 2.0 (max saturation), with 1.0 as the normal level", [this](float& saturation){
        setSaturation(saturation);
    });

    return true;
}

void SaturationFilter::setSaturation(float saturation) {
    _saturation = saturation;
    if (_saturation > 2.0) _saturation = 2.0;
    else if (_saturation < 0.0) _saturation = 0.0;
}

bool SaturationFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("saturation", _saturation);
    return Filter::proceed(bUpdateTargets);
}

