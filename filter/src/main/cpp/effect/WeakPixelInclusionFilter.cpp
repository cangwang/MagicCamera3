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
     float centerIntensity = texture(colorMap, vTexCoord).r;
     
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
    Log("WeakPixelInclusionFilter","init");
    if (initWithFragmentShaderString(kWeakPixelInclusionFragmentShaderString)) {
        return true;
    }
    return false;
}


NS_GI_END
