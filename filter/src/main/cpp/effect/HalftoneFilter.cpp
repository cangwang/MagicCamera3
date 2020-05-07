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

#include "HalftoneFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(HalftoneFilter)

const std::string kHalftoneFragmentShaderString = SHADER_STRING
(
 uniform highp float pixelSize;
 uniform highp float aspectRatio;
 
 uniform sampler2D colorMap;
 varying highp vec2 vTexCoord;
 
 const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
 
 void main()
 {
     highp vec2 pixelSizeVec = vec2(pixelSize, pixelSize / aspectRatio);
     highp vec2 samplePos = floor(vTexCoord / pixelSizeVec) * pixelSizeVec + 0.5 * pixelSizeVec;
     
     highp vec2 textureCoordinateToUse = vec2(vTexCoord.x, (vTexCoord.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
     highp vec2 adjustedSamplePos = vec2(samplePos.x, (samplePos.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
     highp float distanceFromSamplePoint = distance(adjustedSamplePos, textureCoordinateToUse);
     lowp vec3 sampledColor = texture2D(colorMap, samplePos ).rgb;
     highp float dotScaling = 1.0 - dot(sampledColor, W);
     
     lowp float checkForPresenceWithinDot = 1.0 - step(distanceFromSamplePoint, (pixelSize * 0.5) * dotScaling);
     
     gl_FragColor = vec4(vec3(checkForPresenceWithinDot), 1.0);
 }
 );



HalftoneFilter* HalftoneFilter::create() {
    HalftoneFilter* ret = new (std::nothrow) HalftoneFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool HalftoneFilter::init() {
    if (!initWithFragmentShaderString(kHalftoneFragmentShaderString)) return false;

    setPixelSize(0.01);
    registerProperty("pixelSize", _pixelSize, "The size of a pixel that you want to pixellate, ranges from 0 to 0.05.", [this](float& pixelSize){
        setPixelSize(pixelSize);
    });
    
    return true;
}
