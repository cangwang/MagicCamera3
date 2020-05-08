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

#include "WeakPixelInclusionFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(WeakPixelInclusionFilter)

const std::string kWeakPixelInclusionFragmentShaderString = SHADER_STRING
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
     float centerIntensity = texture2D(colorMap, vTexCoord).r;
     
     float pixelIntensitySum = bottomLeftIntensity + topRightIntensity + topLeftIntensity + bottomRightIntensity + leftIntensity + rightIntensity + bottomIntensity + topIntensity + centerIntensity;
     float sumTest = step(1.5, pixelIntensitySum);
     float pixelTest = step(0.01, centerIntensity);
     
     gl_FragColor = vec4(vec3(sumTest * pixelTest), 1.0);
 }
 );


WeakPixelInclusionFilter* WeakPixelInclusionFilter::create() {
    WeakPixelInclusionFilter* ret = new (std::nothrow) WeakPixelInclusionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool WeakPixelInclusionFilter::init() {
    if (initWithFragmentShaderString(kWeakPixelInclusionFragmentShaderString)) {
        return true;
    }
    return false;
}


NS_GI_END
