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

#include "NonMaximumSuppressionFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(NonMaximumSuppressionFilter)

const std::string kNonMaximumSuppressionShaderString = SHADER_STRING
(
 precision mediump float;
 uniform sampler2D colorMap;
 
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
     lowp vec4 centerColor = texture2D(colorMap, vTexCoord);
     
     // Use a tiebreaker for pixels to the left and immediately above this one
     lowp float multiplier = 1.0 - step(centerColor.r, topIntensity);
     multiplier = multiplier * (1.0 - step(centerColor.r, topLeftIntensity));
     multiplier = multiplier * (1.0 - step(centerColor.r, leftIntensity));
     multiplier = multiplier * (1.0 - step(centerColor.r, bottomLeftIntensity));
     
     lowp float maxValue = max(centerColor.r, bottomIntensity);
     maxValue = max(maxValue, bottomRightIntensity);
     maxValue = max(maxValue, rightIntensity);
     maxValue = max(maxValue, topRightIntensity);
     
     gl_FragColor = vec4((centerColor.rgb * step(maxValue, centerColor.r) * multiplier), 1.0);

 }
 );


NonMaximumSuppressionFilter* NonMaximumSuppressionFilter::create() {
    NonMaximumSuppressionFilter* ret = new (std::nothrow) NonMaximumSuppressionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool NonMaximumSuppressionFilter::init() {
    if (initWithFragmentShaderString(kNonMaximumSuppressionShaderString)) {
        return true;
    }
    return false;
}


NS_GI_END
