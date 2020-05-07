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

#include "RGBFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(RGBFilter)

const std::string kRGBFragmentShaderString = SHADER_STRING
(
    uniform sampler2D colorMap;
    uniform highp float redAdjustment;
    uniform highp float greenAdjustment;
    uniform highp float blueAdjustment;
    varying highp vec2 vTexCoord;
 
    void main()
    {
        lowp vec4 color = texture2D(colorMap, vTexCoord);
        gl_FragColor = vec4(color.r * redAdjustment, color.g * greenAdjustment, color.b * blueAdjustment, color.a);
    }
);


RGBFilter* RGBFilter::create() {
    RGBFilter* ret = new (std::nothrow) RGBFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool RGBFilter::init() {
    if (!initWithFragmentShaderString(kRGBFragmentShaderString)) return false;

    _redAdjustment = 1.0;
    _greenAdjustment = 1.0;
    _blueAdjustment = 1.0;
    
    registerProperty("redAdjustment", _redAdjustment, "The red adjustment of the image.The range is from 0.0 up, with 1.0 as the default.", [this](float& redAdjustment){
        setRedAdjustment(redAdjustment);
    });
    
    registerProperty("greenAdjustment", _greenAdjustment, "The green adjustment of the image.The range is from 0.0 up, with 1.0 as the default.", [this](float& greenAdjustment){
        setGreenAdjustment(greenAdjustment);
    });
    
    registerProperty("blueAdjustment", _blueAdjustment, "The blue adjustment of the image.The range is from 0.0 up, with 1.0 as the default.", [this](float& blueAdjustment){
        setBlueAdjustment(blueAdjustment);
    });

    return true;
}

void RGBFilter::setRedAdjustment(float redAdjustment) {
    _redAdjustment = redAdjustment;
    if (_redAdjustment < 0.0) _redAdjustment = 0.0;
}

void RGBFilter::setGreenAdjustment(float greenAdjustment) {
    _greenAdjustment = greenAdjustment;
    if (_greenAdjustment < 0.0) _greenAdjustment = 0.0;
}

void RGBFilter::setBlueAdjustment(float blueAdjustment) {
    _blueAdjustment = blueAdjustment;
    if (_blueAdjustment < 0.0) _blueAdjustment = 0.0;
}
bool RGBFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("redAdjustment", _redAdjustment);
    _filterProgram->setUniformValue("greenAdjustment", _greenAdjustment);
    _filterProgram->setUniformValue("blueAdjustment", _blueAdjustment);
    return Filter::proceed(bUpdateTargets);
}

