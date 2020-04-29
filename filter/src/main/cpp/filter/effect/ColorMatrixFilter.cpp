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

#include "ColorMatrixFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(ColorMatrixFilter)

const std::string kColorMatrixFragmentShaderString = SHADER_STRING
(
 uniform sampler2D colorMap;
 uniform lowp mat4 colorMatrix;
 uniform lowp float intensity;
 
 varying highp vec2 vTexCoord;
 
 void main()
 {
     lowp vec4 textureColor = texture2D(colorMap, vTexCoord);
     lowp vec4 outputColor = textureColor * colorMatrix;
     
     gl_FragColor = (intensity * outputColor) + ((1.0 - intensity) * textureColor);
 }
);


const std::string kBrightnessFragmentShaderString = SHADER_STRING
(
 uniform sampler2D colorMap;
 uniform lowp float brightness;
 varying highp vec2 vTexCoord;
 
 void main()
 {
     lowp vec4 color = texture2D(colorMap, vTexCoord);
     gl_FragColor = vec4((color.rgb + vec3(brightness)), color.w);
 }
 );

ColorMatrixFilter::ColorMatrixFilter()
:_intensity(1.0)
,_colorMatrix(Matrix4::IDENTITY)
{

}

ColorMatrixFilter* ColorMatrixFilter::create() {
    ColorMatrixFilter* ret = new (std::nothrow) ColorMatrixFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool ColorMatrixFilter::init() {
    if ( !Filter::initWithFragmentShaderString(kColorMatrixFragmentShaderString)) return false;

    registerProperty("intensity", _intensity, "The percentage of color applied by color matrix with range between 0 and 1.", [this](float& intensity){
        if (intensity > 1.0) intensity = 1.0;
        else if (intensity < 0.0) intensity = 0.0;
        setIntensity(1.0);
    });

    // todo register paoperty of color matrix

    return true;
}

bool ColorMatrixFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("intensity", _intensity);
    _filterProgram->setUniformValue("colorMatrix", _colorMatrix);
    return Filter::proceed(bUpdateTargets);
}


NS_GI_END
