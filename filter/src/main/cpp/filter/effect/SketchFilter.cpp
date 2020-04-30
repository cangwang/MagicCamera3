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

#include "SketchFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(SketchFilter)

const std::string kSketchFilterFragmentShaderString = SHADER_STRING
(
 precision mediump float;
 uniform sampler2D colorMap;
 uniform float edgeStrength;
 
 varying vec2 vTexCoord;
 varying vec2 vLeftTexCoord;
 varying vec2 vRightTexCoord;
 
 varying vec2 vTopTexCoord;
 varying vec2 vTopLeftTexCoord;
 varying vec2 vTopRightTexCoord;
 
 varying vec2 vBottomTexCoord;
 varying vec2 vBottomLeftTexCoord;
 varying vec2 vBottomRightTexCoord;

 void main()
 {
     float bottomLeftIntensity = texture2D(colorMap, vBottomLeftTexCoord).r;
     float topRightIntensity = texture2D(colorMap, vTopRightTexCoord).r;
     float topLeftIntensity = texture2D(colorMap, vTopLeftTexCoord).r;
     float bottomRightIntensity = texture2D(colorMap, vBottomRightTexCoord).r;
     float leftIntensity = texture2D(colorMap, vLeftTexCoord).r;
     float rightIntensity = texture2D(colorMap, vRightTexCoord).r;
     float bottomIntensity = texture2D(colorMap, vBottomTexCoord).r;
     float topIntensity = texture2D(colorMap, vTopTexCoord).r;
     float h = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensity;
     float v = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity + 2.0 * rightIntensity + topRightIntensity;
     
     float mag = 1.0 - length(vec2(h, v)) * edgeStrength;
     gl_FragColor = vec4(vec3(mag), 1.0);
 }
 );

SketchFilter::SketchFilter()
:_grayscaleFilter(0)
,_sketchFilter(0)
{
    
}

SketchFilter::~SketchFilter()
{
    if (_grayscaleFilter) {
        _grayscaleFilter->release();
        _grayscaleFilter = 0;
    }
    if (_sketchFilter) {
        _sketchFilter->release();
        _sketchFilter = 0;
    }
}

SketchFilter* SketchFilter::create() {
    SketchFilter* ret = new (std::nothrow) SketchFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    
    return ret;
}

bool SketchFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }

    _grayscaleFilter = GrayscaleFilter::create();
    _sketchFilter = _SketchFilter::create();
    _grayscaleFilter->addTarget(_sketchFilter);
    addFilter(_grayscaleFilter);
    
    
    _edgeStrength = 1.0;
    registerProperty("edgeStrength", _edgeStrength, "The edge strength of sobel edge detection filter", [this](float& edgeStrength){
        _sketchFilter->setEdgeStrength(edgeStrength);
    });
    
    return true;
}

_SketchFilter* _SketchFilter::create() {
    _SketchFilter* ret = new (std::nothrow) _SketchFilter();
    if (!ret || !ret->init()) {
        delete ret;
        ret = 0;
    }
    
    return ret;
}

bool _SketchFilter::init() {
    if (!initWithFragmentShaderString(kSketchFilterFragmentShaderString)) {
        return false;
    }
    _edgeStrength = 1.0;
    return true;
}

void _SketchFilter::setEdgeStrength(float edgeStrength) {
    _edgeStrength = edgeStrength;
}

bool _SketchFilter::proceed(bool bUpdateTargets/* = true*/) {
    float texelWidth = 1.0 / _framebuffer->getWidth();
    float texelHeight = 1.0 / _framebuffer->getHeight();
    
    Framebuffer* inputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    RotationMode inputRotation = _inputFramebuffers.begin()->second.rotationMode;
    if (rotationSwapsSize(inputRotation)){
        texelWidth = 1.0 / _framebuffer->getHeight();
        texelHeight = 1.0 / _framebuffer->getWidth();
    }
    
    _filterProgram->setUniformValue("texelWidth", texelWidth);
    _filterProgram->setUniformValue("texelHeight", texelHeight);
    _filterProgram->setUniformValue("edgeStrength", _edgeStrength);
    return NearbySampling3x3Filter::proceed(bUpdateTargets);
}


NS_GI_END
