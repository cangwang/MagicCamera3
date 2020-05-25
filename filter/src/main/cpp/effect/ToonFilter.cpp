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
     
     float mag = length(vec2(h, v));
     
     vec4 color = texture2D(colorMap, vTexCoord);
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

