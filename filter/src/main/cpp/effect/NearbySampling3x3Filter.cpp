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

#include "NearbySampling3x3Filter.hpp"



NS_GI_BEGIN



const std::string kNearbySampling3x3SamplingVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 texCoord;
 
 uniform float texelWidth;
 uniform float texelHeight;
 
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
     gl_Position = position;
     
     vec2 widthStep = vec2(texelWidth, 0.0);
     vec2 heightStep = vec2(0.0, texelHeight);
     vec2 widthHeightStep = vec2(texelWidth, texelHeight);
     vec2 widthNegativeHeightStep = vec2(texelWidth, -texelHeight);
     
     vTexCoord = texCoord.xy;
     vLeftTexCoord = texCoord.xy - widthStep;
     vRightTexCoord = texCoord.xy + widthStep;
     
     vTopTexCoord = texCoord.xy - heightStep;
     vTopLeftTexCoord = texCoord.xy - widthHeightStep;
     vTopRightTexCoord = texCoord.xy + widthNegativeHeightStep;
     
     vBottomTexCoord = texCoord.xy + heightStep;
     vBottomLeftTexCoord = texCoord.xy - widthNegativeHeightStep;
     vBottomRightTexCoord = texCoord.xy + widthHeightStep;
 }
 );


bool NearbySampling3x3Filter::initWithFragmentShaderString(const std::string& fragmentShaderSource, int inputNumber/* = 1*/) {
    if(Filter::initWithShaderString(kNearbySampling3x3SamplingVertexShaderString, fragmentShaderSource)) {
        _texelSizeMultiplier = 1.0;
        _texelWidthUniform = _filterProgram->getUniformLocation("texelWidth");
        _texelHeightUniform = _filterProgram->getUniformLocation("texelHeight");
        
        registerProperty("texelSizeMultiplier", _texelSizeMultiplier, "", [this](float& texelSizeMultiplier){
            setTexelSizeMultiplier(texelSizeMultiplier);
        });
        
        return true;
    }
    return false;
}


bool NearbySampling3x3Filter::proceed(bool bUpdateTargets/* = true*/) {

    float texelWidth = _texelSizeMultiplier / _framebuffer->getWidth();
    float texelHeight = _texelSizeMultiplier / _framebuffer->getHeight();

    RotationMode inputRotation = _inputFramebuffers.begin()->second.rotationMode;
    if (rotationSwapsSize(inputRotation)){
        texelWidth = _texelSizeMultiplier / _framebuffer->getHeight();
        texelHeight = _texelSizeMultiplier / _framebuffer->getWidth();
    }
    
    _filterProgram->setUniformValue(_texelWidthUniform, texelWidth);
    _filterProgram->setUniformValue(_texelHeightUniform, texelHeight);
    return Filter::proceed(bUpdateTargets);
}

void NearbySampling3x3Filter::setTexelSizeMultiplier(float texelSizeMultiplier) {
    if (texelSizeMultiplier > 0)
        _texelSizeMultiplier = texelSizeMultiplier;
}

NS_GI_END
