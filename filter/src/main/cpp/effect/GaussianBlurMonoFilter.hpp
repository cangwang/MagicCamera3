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

#ifndef GaussianBlurMonoFilter_hpp
#define GaussianBlurMonoFilter_hpp

#include "../source/macros.h"
#include "FilterGroup.hpp"

NS_GI_BEGIN

class GaussianBlurMonoFilter : public Filter {
public:
    enum Type {HORIZONTAL, VERTICAL};
    
    static GaussianBlurMonoFilter* create(Type type = HORIZONTAL, int radius = 4, float sigma = 2.0);
    bool init(int radius, float sigma);
    
    void setRadius(int radius);
    void setSigma(float sigma);
    
    virtual bool proceed(bool bUpdateTargets = true) override;
protected:
    GaussianBlurMonoFilter(Type type = HORIZONTAL);
    Type _type;
    int _radius;
    float _sigma;

private:
    virtual std::string _generateVertexShaderString(int radius, float sigma);
    virtual std::string _generateFragmentShaderString(int radius, float sigma);
    
    virtual std::string _generateOptimizedVertexShaderString(int radius, float sigma);
    virtual std::string _generateOptimizedFragmentShaderString(int radius, float sigma);
};


NS_GI_END

#endif /* GaussianBlurMonoFilter_hpp */
