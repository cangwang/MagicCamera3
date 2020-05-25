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

#ifndef SingleComponentGaussianBlurMonoFilter_hpp
#define SingleComponentGaussianBlurMonoFilter_hpp

#include "../source/macros.h"
#include "GaussianBlurMonoFilter.hpp"

NS_GI_BEGIN

class SingleComponentGaussianBlurMonoFilter : public GaussianBlurMonoFilter {
public:
    
    static SingleComponentGaussianBlurMonoFilter* create(Type type = HORIZONTAL, int radius = 4, float sigma = 2.0);
    
protected:
    SingleComponentGaussianBlurMonoFilter(Type type = HORIZONTAL);

private:
    std::string _generateOptimizedVertexShaderString(int radius, float sigma) override;
    std::string _generateOptimizedFragmentShaderString(int radius, float sigma) override;
};


NS_GI_END

#endif /* SingleComponentGaussianBlurMonoFilter_hpp */
