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

#ifndef NearbySampling3x3Filter_hpp
#define NearbySampling3x3Filter_hpp

#include "../source/macros.h"
#include "Filter.hpp"

NS_GI_BEGIN

extern const std::string kNearbySampling3x3SamplingVertexShaderString;

class NearbySampling3x3Filter : public Filter {
public:
    virtual bool initWithFragmentShaderString(const std::string& fragmentShaderSource, int inputNumber = 1) override;
    virtual bool proceed(bool bUpdateTargets = true) override;
    
    void setTexelSizeMultiplier(float texelSizeMultiplier);
protected:
    NearbySampling3x3Filter() {};
    
    float _texelSizeMultiplier;
    GLuint _texelWidthUniform;
    GLuint _texelHeightUniform;
};

NS_GI_END

#endif /* NearbySampling3x3Filter_hpp */
