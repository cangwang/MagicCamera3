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

#include "SobelEdgeDetectionFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(SobelEdgeDetectionFilter)

//   Code from "Graphics Shaders: Theory and Practice" by M. Bailey and S. Cunningham
const std::string kSobelEdgeDetectionFragmentShaderString = SHADER_STRING
(
 precision mediump float;
 uniform sampler2D colorMap;
 uniform float edgeStrength;
 
 in vec2 vTexCoord;
 in vec2 vLeftTexCoord;
 in vec2 vRightTexCoord;
 
 in vec2 vTopTexCoord;
 in vec2 vTopLeftTexCoord;
 in vec2 vTopRightTexCoord;
 
 in vec2 vBottomTexCoord;
 in vec2 vBottomLeftTexCoord;
 in vec2 vBottomRightTexCoord;

 out vec4 gl_FragColor;

 void main()
 {
     float bottomLeftIntensity = texture(colorMap, vBottomLeftTexCoord).r;
     float topRightIntensity = texture(colorMap, vTopRightTexCoord).r;
     float topLeftIntensity = texture(colorMap, vTopLeftTexCoord).r;
     float bottomRightIntensity = texture(colorMap, vBottomRightTexCoord).r;
     float leftIntensity = texture(colorMap, vLeftTexCoord).r;
     float rightIntensity = texture(colorMap, vRightTexCoord).r;
     float bottomIntensity = texture(colorMap, vBottomTexCoord).r;
     float topIntensity = texture(colorMap, vTopTexCoord).r;
     float h = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensity;
     float v = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity + 2.0 * rightIntensity + topRightIntensity;
     
     float mag = length(vec2(h, v)) * edgeStrength;
     gl_FragColor = vec4(vec3(mag), 1.0);
 }
 );

SobelEdgeDetectionFilter::SobelEdgeDetectionFilter()
:_grayscaleFilter(0)
,_sobelEdgeDetectionFilter(0)
{
}

SobelEdgeDetectionFilter::~SobelEdgeDetectionFilter() {
    if (_grayscaleFilter) {
        _grayscaleFilter->release();
        _grayscaleFilter = 0;
    }
    if (_sobelEdgeDetectionFilter) {
        _sobelEdgeDetectionFilter->release();
        _sobelEdgeDetectionFilter = 0;
    }
}

SobelEdgeDetectionFilter* SobelEdgeDetectionFilter::create() {
    SobelEdgeDetectionFilter* ret = new (std::nothrow) SobelEdgeDetectionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    
    return ret;
}

bool SobelEdgeDetectionFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }

    _grayscaleFilter = GrayscaleFilter::create();
    _sobelEdgeDetectionFilter = _SobelEdgeDetectionFilter::create();
    _grayscaleFilter->addTarget(_sobelEdgeDetectionFilter);
    addFilter(_grayscaleFilter);
    
    
    _edgeStrength = 1.0;
    registerProperty("edgeStrength", _edgeStrength, "The edge strength of sobel edge detection filter", [this](float& edgeStrength){
        _sobelEdgeDetectionFilter->setEdgeStrength(edgeStrength);
    });
    
    return true;
}

_SobelEdgeDetectionFilter* _SobelEdgeDetectionFilter::create() {
    _SobelEdgeDetectionFilter* ret = new (std::nothrow) _SobelEdgeDetectionFilter();
    if (!ret || !ret->init()) {
        delete ret;
        ret = 0;
    }
    
    return ret;
}

bool _SobelEdgeDetectionFilter::init() {
    if (!initWithFragmentShaderString(kSobelEdgeDetectionFragmentShaderString)) {
        return false;
    }
    _edgeStrength = 1.0;
    return true;
}

void _SobelEdgeDetectionFilter::setEdgeStrength(float edgeStrength) {
    _edgeStrength = edgeStrength;
}

bool _SobelEdgeDetectionFilter::proceed(bool bUpdateTargets/* = true*/) {
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
