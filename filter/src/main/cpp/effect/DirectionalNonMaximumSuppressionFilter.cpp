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

#include "DirectionalNonMaximumSuppressionFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(DirectionalNonMaximumSuppressionFilter)

const std::string kDirectionalNonmaximumSuppressionFragmentShaderString = SHADER_STRING
(
 precision mediump float;
 
 uniform sampler2D colorMap;
 uniform highp float texelWidth;
 uniform highp float texelHeight;
 uniform mediump float upperThreshold;
 uniform mediump float lowerThreshold;
 
 varying highp vec2 vTexCoord;
 
 void main()
 {
     vec3 currentGradientAndDirection = texture2D(colorMap, vTexCoord).rgb;
     vec2 gradientDirection = ((currentGradientAndDirection.gb * 2.0) - 1.0) * vec2(texelWidth, texelHeight);
     
     float firstSampledGradientMagnitude = texture2D(colorMap, vTexCoord + gradientDirection).r;
     float secondSampledGradientMagnitude = texture2D(colorMap, vTexCoord - gradientDirection).r;
     
     float multiplier = step(firstSampledGradientMagnitude, currentGradientAndDirection.r);
     multiplier = multiplier * step(secondSampledGradientMagnitude, currentGradientAndDirection.r);
     
     float thresholdCompliance = smoothstep(lowerThreshold, upperThreshold, currentGradientAndDirection.r);
     multiplier = multiplier * thresholdCompliance;
     
     gl_FragColor = vec4(multiplier, multiplier, multiplier, 1.0);
 }
 );


DirectionalNonMaximumSuppressionFilter* DirectionalNonMaximumSuppressionFilter::create() {
    DirectionalNonMaximumSuppressionFilter* ret = new (std::nothrow) DirectionalNonMaximumSuppressionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool DirectionalNonMaximumSuppressionFilter::init() {
    if (initWithFragmentShaderString(kDirectionalNonmaximumSuppressionFragmentShaderString)) {
        _texelWidthUniform = _filterProgram->getUniformLocation("texelWidth");
        _texelHeightUniform = _filterProgram->getUniformLocation("texelWidth");
        
        _filterProgram->setUniformValue("upperThreshold", (float)0.5);
        _filterProgram->setUniformValue("lowerThreshold", (float)0.1);
        
        return true;
    }
    return false;
}


bool DirectionalNonMaximumSuppressionFilter::proceed(bool bUpdateTargets/* = true*/) {
    float texelWidth = 1.0 / _framebuffer->getWidth();
    float texelHeight = 1.0 / _framebuffer->getHeight();
    
    Framebuffer* inputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    RotationMode inputRotation = _inputFramebuffers.begin()->second.rotationMode;
    
    if (rotationSwapsSize(inputRotation)){
        texelWidth = 1.0 / _framebuffer->getHeight();
        texelHeight = 1.0 / _framebuffer->getWidth();
    }
    
    _filterProgram->setUniformValue(_texelWidthUniform, texelWidth);
    _filterProgram->setUniformValue(_texelHeightUniform, texelHeight);
    
    return Filter::proceed(bUpdateTargets);
}



NS_GI_END
