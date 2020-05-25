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

#include "ExposureFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(ExposureFilter)

const std::string kExposureFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform lowp float exposure;
    varying highp vec2 vTexCoord;
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        gl_FragColor = vec4(color.rgb * pow(2.0, exposure), color.a);
    }
);


ExposureFilter* ExposureFilter::create() {
    ExposureFilter* ret = new (std::nothrow) ExposureFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool ExposureFilter::init() {
    if (!initWithFragmentShaderString(kExposureFragmentShaderString)) return false;

    _exposure = 0.0;
    registerProperty("exposure", _exposure, "The exposure of the image. Exposure ranges from -10.0 to 10.0 (max contrast), with 0.0 as the normal level", [this](float& exposure){
        setExposure(exposure);
    });

    return true;
}

void ExposureFilter::setExposure(float exposure) {
    _exposure = exposure;
    if (_exposure > 10.0) _exposure = 10.0;
    else if (_exposure < -10.0) _exposure = -10.0;
}

bool ExposureFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("exposure", _exposure);
    return Filter::proceed(bUpdateTargets);
}

