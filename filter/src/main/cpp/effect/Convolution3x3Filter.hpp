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

#ifndef Convolution3x3Filter_hpp
#define Convolution3x3Filter_hpp

#include "../source/macros.h"
#include "NearbySampling3x3Filter.hpp"
#include "../source/math.hpp"

NS_GI_BEGIN

class Convolution3x3Filter : public NearbySampling3x3Filter {
public:
    virtual bool init();
    virtual bool proceed(bool bUpdateTargets = true) override;
protected:
    Convolution3x3Filter() {};
    
    
    //The convolution kernel is a 3x3 matrix of values to apply to the pixel and its 8 surrounding pixels.
    Matrix3 _convolutionKernel;
};

NS_GI_END

#endif /* Convolution3x3Filter_hpp */
