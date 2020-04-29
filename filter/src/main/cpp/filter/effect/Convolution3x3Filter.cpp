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

#include "Convolution3x3Filter.hpp"



NS_GI_BEGIN



const std::string kConvolution3x3FragmentShaderString = SHADER_STRING
(
 precision highp float;
 uniform sampler2D colorMap;
 uniform mediump mat3 convolutionMatrix;
 
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
     mediump vec3 bottomColor = texture2D(colorMap, vBottomTexCoord).rgb;
     mediump vec3 bottomLeftColor = texture2D(colorMap, vBottomLeftTexCoord).rgb;
     mediump vec3 bottomRightColor = texture2D(colorMap, vBottomRightTexCoord).rgb;
     mediump vec4 centerColor = texture2D(colorMap, vTexCoord);
     mediump vec3 leftColor = texture2D(colorMap, vLeftTexCoord).rgb;
     mediump vec3 rightColor = texture2D(colorMap, vRightTexCoord).rgb;
     mediump vec3 topColor = texture2D(colorMap, vTopTexCoord).rgb;
     mediump vec3 topRightColor = texture2D(colorMap, vTopRightTexCoord).rgb;
     mediump vec3 topLeftColor = texture2D(colorMap, vTopLeftTexCoord).rgb;
     
     mediump vec3 resultColor = topLeftColor * convolutionMatrix[0][0] + topColor * convolutionMatrix[0][1] + topRightColor * convolutionMatrix[0][2];
     resultColor += leftColor * convolutionMatrix[1][0] + centerColor.rgb * convolutionMatrix[1][1] + rightColor * convolutionMatrix[1][2];
     resultColor += bottomLeftColor * convolutionMatrix[2][0] + bottomColor * convolutionMatrix[2][1] + bottomRightColor * convolutionMatrix[2][2];
     
     gl_FragColor = vec4(resultColor, centerColor.a);
 }
 );


bool Convolution3x3Filter::init() {
    if(!NearbySampling3x3Filter::initWithFragmentShaderString(kConvolution3x3FragmentShaderString)) return false;
    
    _convolutionKernel.set(0.f, 0.f, 0.f,
                           0.f, 1.f, 0.f,
                           0.f, 0.f, 0.f);
    
    
    return true;
}

bool Convolution3x3Filter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("convolutionMatrix", _convolutionKernel);
    return NearbySampling3x3Filter::proceed(bUpdateTargets);
}

NS_GI_END
