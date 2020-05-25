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

#include "ContrastFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(ContrastFilter)

const std::string kContrastFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform lowp float contrast;
    varying highp vec2 vTexCoord;
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        gl_FragColor = vec4(((color.rgb - vec3(0.5)) * contrast + vec3(0.5)), color.a);
    }
);


ContrastFilter* ContrastFilter::create() {
    ContrastFilter* ret = new (std::nothrow) ContrastFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool ContrastFilter::init() {
    if (!initWithFragmentShaderString(kContrastFragmentShaderString)) return false;

    _contrast = 1.0;
    registerProperty("contrast", _contrast, "The contrast of the image. Contrast ranges from 0.0 to 4.0 (max contrast), with 1.0 as the normal level", [this](float& contrast){
        setContrast(contrast);
    });

    return true;
}

void ContrastFilter::setContrast(float contrast) {
    _contrast = contrast;
    if (_contrast > 4.0) _contrast = 4.0;
    else if (_contrast < 0.0) _contrast = 0.0;
}

bool ContrastFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("contrast", _contrast);
    return Filter::proceed(bUpdateTargets);
}

