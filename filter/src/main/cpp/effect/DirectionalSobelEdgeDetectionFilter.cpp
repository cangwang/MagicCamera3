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

#include "DirectionalSobelEdgeDetectionFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(DirectionalSobelEdgeDetectionFilter)

const std::string kDirectionalSobelEdgeDetectionFragmentShaderString = SHADER_STRING
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
     
     vec2 gradientDirection;
     gradientDirection.x = -bottomLeftIntensity - 2.0 * leftIntensity - topLeftIntensity + bottomRightIntensity + 2.0 * rightIntensity + topRightIntensity;
     gradientDirection.y = -topLeftIntensity - 2.0 * topIntensity - topRightIntensity + bottomLeftIntensity + 2.0 * bottomIntensity + bottomRightIntensity;
     
     float gradientMagnitude = length(gradientDirection);
     vec2 normalizedDirection = normalize(gradientDirection);
     normalizedDirection = sign(normalizedDirection) * floor(abs(normalizedDirection) + 0.617316); // Offset by 1-sin(pi/8) to set to 0 if near axis, 1 if away
     normalizedDirection = (normalizedDirection + 1.0) * 0.5; // Place -1.0 - 1.0 within 0 - 1.0
     
     gl_FragColor = vec4(gradientMagnitude, normalizedDirection.x, normalizedDirection.y, 1.0);
 }
 );


DirectionalSobelEdgeDetectionFilter* DirectionalSobelEdgeDetectionFilter::create() {
    DirectionalSobelEdgeDetectionFilter* ret = new (std::nothrow) DirectionalSobelEdgeDetectionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool DirectionalSobelEdgeDetectionFilter::init() {
    if (initWithFragmentShaderString(kDirectionalSobelEdgeDetectionFragmentShaderString)) {
        return true;
    }
    return false;
}


NS_GI_END
