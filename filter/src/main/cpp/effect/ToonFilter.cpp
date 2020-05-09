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

#include "ToonFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(ToonFilter)

const std::string kToonFragmentShaderString = SHADER_STRING
(
 precision mediump float;
 uniform sampler2D colorMap;
 uniform float threshold;
 uniform float quantizationLevels;
 
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
     
     float mag = length(vec2(h, v));
     
     vec4 color = texture(colorMap, vTexCoord);
     vec3 posterizedImageColor = (floor(color.rgb * quantizationLevels) + 0.5) / quantizationLevels;
     
     float thresholdTest = 1.0 - step(threshold, mag);
     
     gl_FragColor = vec4(posterizedImageColor * thresholdTest, color.a);
 }
 );


ToonFilter* ToonFilter::create() {
    ToonFilter* ret = new (std::nothrow) ToonFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool ToonFilter::init() {
    if (!initWithFragmentShaderString(kToonFragmentShaderString)) return false;

    _threshold = 0.2;
    registerProperty("threshold", _threshold, "The threshold at which to apply the edges", [this](float& threshold){
        setThreshold(threshold);
    });

    _quantizationLevels = 10.0;
    registerProperty("quantizationLevels", _quantizationLevels, "The levels of quantization for the posterization of colors within the scene", [this](float& quantizationLevels){
        setQuantizatinLevels(quantizationLevels);
    });
    
    return true;
}

void ToonFilter::setThreshold(float threshold) {
    _threshold = threshold;
}

void ToonFilter::setQuantizatinLevels(float quantizationLevels) {
    _quantizationLevels = quantizationLevels;
}

bool ToonFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("threshold", _threshold);
    _filterProgram->setUniformValue("quantizationLevels", _quantizationLevels);
    return NearbySampling3x3Filter::proceed(bUpdateTargets);
}

