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

#include "SingleComponentGaussianBlurMonoFilter.hpp"
#include <cmath>

NS_GI_BEGIN

REGISTER_FILTER_CLASS(SingleComponentGaussianBlurMonoFilter)

SingleComponentGaussianBlurMonoFilter::SingleComponentGaussianBlurMonoFilter(Type type/* = HORIZONTAL*/)
:GaussianBlurMonoFilter(type)
{
}

SingleComponentGaussianBlurMonoFilter* SingleComponentGaussianBlurMonoFilter::create(Type type/* = HORIZONTAL*/, int radius/* = 4*/, float sigma/* = 2.0*/) {
    SingleComponentGaussianBlurMonoFilter* ret = new (std::nothrow) SingleComponentGaussianBlurMonoFilter(type);
    if (ret && !ret->init(radius, sigma)) {
        delete ret;
        ret = 0;
    }
    return ret;
}

std::string SingleComponentGaussianBlurMonoFilter::_generateOptimizedVertexShaderString(int radius, float sigma)
{
    if (radius < 1 || sigma <= 0.0)
    {
        return kDefaultVertexShader;
    }
    
    // 1. generate the normal Gaussian weights for a given sigma
    float* standardGaussianWeights = new float[radius + 1];
    float sumOfWeights = 0.0;
    for (int i = 0; i < radius + 1; ++i)
    {
        standardGaussianWeights[i] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(i, 2.0) / (2.0 * pow(sigma, 2.0)));
        if (i == 0)
            sumOfWeights += standardGaussianWeights[i];
        else
            sumOfWeights += 2.0 * standardGaussianWeights[i];
    }
    
    // 2. normalize these weights to prevent the clipping of the Gaussian curve at the end of the discrete samples from reducing luminance
    for (int i = 0; i < radius + 1; ++i)
    {
        standardGaussianWeights[i] = standardGaussianWeights[i] / sumOfWeights;
    }
    
    // 3. From these weights we calculate the offsets to read interpolated values from
    int numberOfOptimizedOffsets = fmin(radius / 2 + (radius % 2), 7);
    float* optimizedGaussianOffsets = new float[numberOfOptimizedOffsets];
    
    for (int i = 0; i < numberOfOptimizedOffsets; ++i)
    {
        GLfloat firstWeight = standardGaussianWeights[i * 2 + 1];
        GLfloat secondWeight = standardGaussianWeights[i * 2 + 2];
        
        GLfloat optimizedWeight = firstWeight + secondWeight;
        
        optimizedGaussianOffsets[i] = (firstWeight * (i * 2 + 1) + secondWeight * (i * 2 + 2)) / optimizedWeight;
    }
    
    std::string shaderStr =
    str_format("\
               attribute vec4 position;\n\
               attribute vec4 texCoord;\n\
               uniform float texelWidthOffset;\n\
               uniform float texelHeightOffset;\n\
               varying highp vec2 blurCoordinates[%d];\n\
               void main()\n\
               {\n\
               gl_Position = position;\n\
               vec2 texelSpacing = vec2(texelWidthOffset, texelHeightOffset);\n\
               ", numberOfOptimizedOffsets * 2 + 1);

    shaderStr = shaderStr + str_format("blurCoordinates[0] = texCoord.xy;\n");
    for (int i = 0; i < numberOfOptimizedOffsets; ++i) {
        shaderStr = shaderStr + str_format(
           "blurCoordinates[%d] = texCoord.xy + texelSpacing * (%f);\n\
            blurCoordinates[%d] = texCoord.xy - texelSpacing * (%f);",
            i * 2 + 1,
            optimizedGaussianOffsets[i],
            i * 2 + 2,
            optimizedGaussianOffsets[i]);
    }
    
    shaderStr += "}\n";
    
    delete[] standardGaussianWeights;
    delete[] optimizedGaussianOffsets;
    
    return shaderStr;
}

std::string SingleComponentGaussianBlurMonoFilter::_generateOptimizedFragmentShaderString(int radius, float sigma)
{
    if (radius < 1 || sigma <= 0.0)
    {
        return kDefaultFragmentShader;
    }
    
    // 1. generate the normal Gaussian weights for a given sigma
    float* standardGaussianWeights = new float[radius + 1];
    float sumOfWeights = 0.0;
    for (int i = 0; i < radius + 1; ++i)
    {
        standardGaussianWeights[i] = (1.0 / sqrt(2.0 * M_PI * pow(sigma, 2.0))) * exp(-pow(i, 2.0) / (2.0 * pow(sigma, 2.0)));
        if (i == 0)
            sumOfWeights += standardGaussianWeights[i];
        else
            sumOfWeights += 2.0 * standardGaussianWeights[i];
    }
    
    // 2. normalize these weights to prevent the clipping of the Gaussian curve at the end of the discrete samples from reducing luminance
    for (int i = 0; i < radius + 1; ++i)
    {
        standardGaussianWeights[i] = standardGaussianWeights[i] / sumOfWeights;
    }
    
    // 3. From these weights we calculate the offsets to read interpolated values from
    int trueNumberOfOptimizedOffsets = radius / 2 + (radius % 2);
    int numberOfOptimizedOffsets = fmin(trueNumberOfOptimizedOffsets, 7);

    std::string shaderStr =
    str_format("\
               uniform sampler2D colorMap;\n\
               uniform highp float texelWidthOffset;\n\
               uniform highp float texelHeightOffset;\n\
               varying highp vec2 blurCoordinates[%d];\n\
               void main()\n\
               {\n\
               lowp float sum = 0.0;\n", numberOfOptimizedOffsets * 2 + 1);
    
    shaderStr += str_format("gl_FragColor += texture2D(colorMap, blurCoordinates[0]) * %f;\n", standardGaussianWeights[0]);
    for (int i = 0; i < numberOfOptimizedOffsets; ++i) {
        float firstWeight = standardGaussianWeights[i * 2 + 1];
        float secondWeight = standardGaussianWeights[i * 2 + 2];
        float optimizedWeight = firstWeight + secondWeight;
        
        shaderStr += str_format("sum += texture2D(colorMap, blurCoordinates[%d]).r * %f;\n", i * 2 + 1, optimizedWeight);
        shaderStr += str_format("sum += texture2D(colorMap, blurCoordinates[%d]).r * %f;\n", i * 2 + 2, optimizedWeight);
    }
    
    // If the number of required samples exceeds the amount we can pass in via varyings, we have to do dependent texture reads in the fragment shader
    if (trueNumberOfOptimizedOffsets > numberOfOptimizedOffsets)
    {
        shaderStr += str_format("highp vec2 texelSpacing = vec2(texelWidthOffset, texelHeightOffset);\n");
        
        for (int i = numberOfOptimizedOffsets; i < trueNumberOfOptimizedOffsets; i++)
        {
            float firstWeight = standardGaussianWeights[i * 2 + 1];
            float secondWeight = standardGaussianWeights[i * 2 + 2];
            
            float optimizedWeight = firstWeight + secondWeight;
            float optimizedOffset = (firstWeight * (i * 2 + 1) + secondWeight * (i * 2 + 2)) / optimizedWeight;
            
            shaderStr += str_format("sum += texture2D(colorMap, blurCoordinates[0] + texelSpacing * %f).r * %f;\n", optimizedOffset, optimizedWeight);
            
            shaderStr += str_format("sum += texture2D(colorMap, blurCoordinates[0] - texelSpacing * %f).r * %f;\n", optimizedOffset, optimizedWeight);
        }
    }

    shaderStr +=
    "gl_FragColor = vec4(sum, sum, sum, 1.0);\n\
    }";
    
    delete[] standardGaussianWeights; standardGaussianWeights = 0;
    return shaderStr;
}

NS_GI_END
