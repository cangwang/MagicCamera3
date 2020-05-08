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

#include "GrayscaleFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(GrayscaleFilter)

const std::string kGrayscaleFragmentShaderString = SHADER_STRING
(
 precision highp float;
 uniform sampler2D colorMap;
 varying highp vec2 vTexCoord;
 
 const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
 
 void main()
 {
     lowp vec4 color = texture2D(colorMap, vTexCoord);
     float luminance = dot(color.rgb, vec3(0.2125, 0.7154, 0.0721));
     gl_FragColor = vec4(vec3(luminance), color.a);
 }
);


GrayscaleFilter* GrayscaleFilter::create() {
    GrayscaleFilter* ret = new (std::nothrow) GrayscaleFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool GrayscaleFilter::init() {
    if (Filter::initWithFragmentShaderString(kGrayscaleFragmentShaderString)) {
        return true;
    }
    return false;
}

bool GrayscaleFilter::proceed(bool bUpdateTargets/* = true*/) {
    return Filter::proceed(bUpdateTargets);
}

NS_GI_END
