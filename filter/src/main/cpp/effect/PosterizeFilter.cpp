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

#include "PosterizeFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(PosterizeFilter)

const std::string kPosterizeFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform highp float colorLevels;
    varying highp vec2 vTexCoord;
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        
        gl_FragColor = floor((color * colorLevels) + vec4(0.5)) / colorLevels;
    }
);


PosterizeFilter* PosterizeFilter::create() {
    PosterizeFilter* ret = new (std::nothrow) PosterizeFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool PosterizeFilter::init() {
    if (!initWithFragmentShaderString(kPosterizeFragmentShaderString)) return false;

    _colorLevels = 10;
    registerProperty("colorLevels", _colorLevels, "The number of color levels to reduce the image space to. This ranges from 1 to 256, with a default of 10.", [this](int& colorLevels){
        setColorLevels(colorLevels);
    });

    return true;
}

void PosterizeFilter::setColorLevels(int colorLevels) {
    _colorLevels = colorLevels;
    if (_colorLevels > 256) _colorLevels = 256;
    else if (_colorLevels < 1) _colorLevels = 1;
}

bool PosterizeFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("colorLevels", (float)_colorLevels);
    return Filter::proceed(bUpdateTargets);
}

