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

#include "CrosshatchFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(CrosshatchFilter)

const std::string kCrosshatchFragmentShaderString = SHADER_STRING
(
 uniform sampler2D colorMap;
 varying highp vec2 vTexCoord;
 uniform highp float crossHatchSpacing;
 uniform highp float lineWidth;
 
 const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
 
 void main()
 {
     highp float luminance = dot(texture2D(colorMap, vTexCoord).rgb, W);
     
     lowp vec4 colorToDisplay = vec4(1.0, 1.0, 1.0, 1.0);
     if (luminance < 1.00)
     {
         if (mod(vTexCoord.x + vTexCoord.y, crossHatchSpacing) <= lineWidth)
         {
             colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
         }
     }
     if (luminance < 0.75)
     {
         if (mod(vTexCoord.x - vTexCoord.y, crossHatchSpacing) <= lineWidth)
         {
             colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
         }
     }
     if (luminance < 0.50)
     {
         if (mod(vTexCoord.x + vTexCoord.y - (crossHatchSpacing / 2.0), crossHatchSpacing) <= lineWidth)
         {
             colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
         }
     }
     if (luminance < 0.3)
     {
         if (mod(vTexCoord.x - vTexCoord.y - (crossHatchSpacing / 2.0), crossHatchSpacing) <= lineWidth)
         {
             colorToDisplay = vec4(0.0, 0.0, 0.0, 1.0);
         }
     }
     
     gl_FragColor = colorToDisplay;
 }

);




CrosshatchFilter* CrosshatchFilter::create() {
    CrosshatchFilter* ret = new (std::nothrow) CrosshatchFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool CrosshatchFilter::init() {
    if (!initWithFragmentShaderString(kCrosshatchFragmentShaderString)) return false;

    
    setCrossHatchSpacing(0.03);
    registerProperty("crossHatchSpacing", _crossHatchSpacing, "The fractional width of the image to use as the spacing for the crosshatch. The default is 0.03.", [this](float& crossHatchSpacing){
        setCrossHatchSpacing(crossHatchSpacing);
    });

    setLineWidth(0.003);
    registerProperty("lineWidth", _lineWidth, "A relative width for the crosshatch lines. The default is 0.003.", [this](float& lineWidth){
        setLineWidth(lineWidth);
    });
    return true;
}

bool CrosshatchFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("crossHatchSpacing", _crossHatchSpacing);
    _filterProgram->setUniformValue("lineWidth", _lineWidth);
    return Filter::proceed(bUpdateTargets);
}

void CrosshatchFilter::setCrossHatchSpacing(float crossHatchSpacing) {
    _crossHatchSpacing = crossHatchSpacing;
}

void CrosshatchFilter::setLineWidth(float lineWidth) {
    _lineWidth = lineWidth;
}

