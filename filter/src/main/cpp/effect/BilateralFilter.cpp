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

#include "BilateralFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(BilateralMonoFilter)

const std::string kBilateralBlurVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 texCoord;
 
 const int GAUSSIAN_SAMPLES = 9;
 
 uniform float texelSpacingU;
 uniform float texelSpacingV;

 varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];
 
 void main()
 {
     gl_Position = position;
     vec2 texelSpacing = vec2(texelSpacingU, texelSpacingV);
     for (int i = 0; i < GAUSSIAN_SAMPLES; i++)
     {
         blurCoordinates[i] = texCoord.xy + texelSpacing * float((i - ((GAUSSIAN_SAMPLES - 1) / 2)));
     }
 }
 );

const std::string kBilateralBlurFragmentShaderString = SHADER_STRING
(
 uniform sampler2D colorMap;
 
 const lowp int GAUSSIAN_SAMPLES = 9;
 
 varying highp vec2 blurCoordinates[GAUSSIAN_SAMPLES];
 
 uniform mediump float distanceNormalizationFactor;
 
 void main()
 {
     lowp vec4 centralColor;
     lowp float gaussianWeightTotal;
     lowp vec4 sum;
     lowp vec4 sampleColor;
     lowp float distanceFromCentralColor;
     lowp float gaussianWeight;
     
     centralColor = texture2D(colorMap, blurCoordinates[4]);
     gaussianWeightTotal = 0.18;
     sum = centralColor * 0.18;
     
     sampleColor = texture2D(colorMap, blurCoordinates[0]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[1]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[2]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[3]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[5]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[6]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[7]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     sampleColor = texture2D(colorMap, blurCoordinates[8]);
     distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
     gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
     gaussianWeightTotal += gaussianWeight;
     sum += sampleColor * gaussianWeight;
     
     gl_FragColor = sum / gaussianWeightTotal;
 }
 );

 BilateralMonoFilter::BilateralMonoFilter(Type type)
 :_type(type)
,_texelSpacingMultiplier(4.0)
,_distanceNormalizationFactor(8.0)
 {
 
 }
 
 BilateralMonoFilter* BilateralMonoFilter::create(Type type/* = HORIZONTAL*/) {
     BilateralMonoFilter* ret = new (std::nothrow) BilateralMonoFilter(type);
     if (!ret || !ret->init()) {
         delete ret;
         ret = 0;
     }
     return ret;
 }

bool BilateralMonoFilter::init() {
    if (Filter::initWithShaderString(kBilateralBlurVertexShaderString, kBilateralBlurFragmentShaderString)) {
        return true;
    }
    return false;
}

bool BilateralMonoFilter::proceed(bool bUpdateTargets/* = true*/) {
    Framebuffer* inputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    RotationMode inputRotation = _inputFramebuffers.begin()->second.rotationMode;
    
    if (rotationSwapsSize(inputRotation))
    {
        if (_type == HORIZONTAL) {
            _filterProgram->setUniformValue("texelSpacingU", (float)0.0);
            _filterProgram->setUniformValue("texelSpacingV", (float)(_texelSpacingMultiplier / _framebuffer->getWidth()));
        } else {
            _filterProgram->setUniformValue("texelSpacingU", (float)(_texelSpacingMultiplier / _framebuffer->getHeight()));
            _filterProgram->setUniformValue("texelSpacingV", (float)0.0);
        }
    } else {
        if (_type == HORIZONTAL) {
            _filterProgram->setUniformValue("texelSpacingU", (float)(_texelSpacingMultiplier / _framebuffer->getWidth()));
            _filterProgram->setUniformValue("texelSpacingV", (float)0.0);
        } else {
            _filterProgram->setUniformValue("texelSpacingU", (float)0.0);
            _filterProgram->setUniformValue("texelSpacingV", (float)(_texelSpacingMultiplier / _framebuffer->getHeight()));
        }
    }
    
    
    _filterProgram->setUniformValue("distanceNormalizationFactor", _distanceNormalizationFactor);
    return Filter::proceed(bUpdateTargets);
}

void BilateralMonoFilter::setTexelSpacingMultiplier(float multiplier) {
    _texelSpacingMultiplier = multiplier;
}

void BilateralMonoFilter::setDistanceNormalizationFactor(float value) {
    _distanceNormalizationFactor = value;
}

REGISTER_FILTER_CLASS(BilateralFilter)

BilateralFilter::BilateralFilter()
:_hBlurFilter(0)
,_vBlurFilter(0)
{
}

BilateralFilter::~BilateralFilter() {
    if (_hBlurFilter) {
        _hBlurFilter->release();
        _hBlurFilter = 0;
    }
    
    if (_vBlurFilter) {
        _vBlurFilter->release();
        _vBlurFilter = 0;
    }
    
}

BilateralFilter* BilateralFilter::create() {
    BilateralFilter* ret = new (std::nothrow) BilateralFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool BilateralFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }
    
    _hBlurFilter = BilateralMonoFilter::create(BilateralMonoFilter::HORIZONTAL);
    _vBlurFilter = BilateralMonoFilter::create(BilateralMonoFilter::VERTICAL);
    _hBlurFilter->addTarget(_vBlurFilter);
    addFilter(_hBlurFilter);

    registerProperty("texelSpacingMultiplier", 4.0, "The texel spacing multiplier.", [this](float& texelSpacingMultiplier){
        setTexelSpacingMultiplier(texelSpacingMultiplier);
    });

    registerProperty("distanceNormalizationFactor", 8.0, "The distance normalization factor.", [this](float& distanceNormalizationFactor){
        setDistanceNormalizationFactor(distanceNormalizationFactor);
    });

    return true;
}

void BilateralFilter::setTexelSpacingMultiplier(float multiplier) {
    _hBlurFilter->setTexelSpacingMultiplier(multiplier);
    _vBlurFilter->setTexelSpacingMultiplier(multiplier);
}

void BilateralFilter::setDistanceNormalizationFactor(float value) {
    _hBlurFilter->setDistanceNormalizationFactor(value);
    _vBlurFilter->setDistanceNormalizationFactor(value);
    
}
NS_GI_END
