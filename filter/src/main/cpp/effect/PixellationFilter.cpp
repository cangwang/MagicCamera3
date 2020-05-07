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

#include "PixellationFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(PixellationFilter)

const std::string kPixellationFragmentShaderString = SHADER_STRING
(
 uniform highp float pixelSize;
 uniform highp float aspectRatio;
 
 uniform sampler2D colorMap;
 varying highp vec2 vTexCoord;
 
 void main()
 {
     highp vec2 pixelSizeVec = vec2(pixelSize, pixelSize / aspectRatio);
     highp vec2 samplePos = floor(vTexCoord / pixelSizeVec) * pixelSizeVec + 0.5 * pixelSizeVec;
     gl_FragColor = texture2D(colorMap, samplePos);
 }
 );


PixellationFilter* PixellationFilter::create() {
    PixellationFilter* ret = new (std::nothrow) PixellationFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool PixellationFilter::init() {
    if (!initWithFragmentShaderString(kPixellationFragmentShaderString)) return false;

    _pixelSize = 0.1;
    registerProperty("pixelSize", _pixelSize, "The size of a pixel that you want to pixellate, ranges from 0 to 1.", [this](float& pixelSize){
        setPixelSize(pixelSize);
    });
    
    return true;
}

void PixellationFilter::setPixelSize(float pixelSize) {
    _pixelSize = pixelSize;
    if (_pixelSize > 1.0) _pixelSize = 1.0;
    else if (_pixelSize < 0.0) _pixelSize = 0.0;
    
}

bool PixellationFilter::proceed(bool bUpdateTargets/* = true*/) {
    
    float aspectRatio = 1.0;
    Framebuffer* firstInputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    aspectRatio = firstInputFramebuffer->getHeight() / (float)(firstInputFramebuffer->getWidth());
    _filterProgram->setUniformValue("aspectRatio", aspectRatio);
    
    float pixelSize = _pixelSize;
    float singlePixelWidth = 1.0 / firstInputFramebuffer->getWidth();
    if (pixelSize < singlePixelWidth)
    {
        pixelSize = singlePixelWidth;
    }
    _filterProgram->setUniformValue("pixelSize", pixelSize);

    return Filter::proceed(bUpdateTargets);
}

