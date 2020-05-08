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

#include "ColorInvertFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(ColorInvertFilter)

const std::string kColorInvertFragmentShaderString = SHADER_STRING
(
 
    uniform sampler2D colorMap;
    varying highp vec2 vTexCoord;

    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        gl_FragColor = vec4((1.0 - color.rgb), color.a);
    }
);


ColorInvertFilter* ColorInvertFilter::create() {
    ColorInvertFilter* ret = new (std::nothrow) ColorInvertFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool ColorInvertFilter::init() {
    if (!Filter::initWithFragmentShaderString(kColorInvertFragmentShaderString)) return false;
    return true;
}


bool ColorInvertFilter::proceed(bool bUpdateTargets/* = true*/) {
    return Filter::proceed(bUpdateTargets);
}


NS_GI_END
