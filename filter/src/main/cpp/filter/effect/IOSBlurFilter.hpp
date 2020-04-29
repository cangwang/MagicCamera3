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

#ifndef IOSBlurFilter_hpp
#define IOSBlurFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "SaturationFilter.hpp"
#include "GaussianBlurFilter.hpp"
#include "LuminanceRangeFilter.hpp"

NS_GI_BEGIN

class IOSBlurFilter : public FilterGroup {
public:
    virtual ~IOSBlurFilter();
    
    static IOSBlurFilter* create();
    bool init();
    
    void setBlurSigma(float blurSigma);
    void setSaturation(float saturation);
    void setRangeReductionFactor(float rangeReductionFactor);
    void setDownSampling(float downSampling);
    
protected:

    IOSBlurFilter();
    SaturationFilter* _saturationFilter;
    GaussianBlurFilter* _blurFilter;
    LuminanceRangeFilter* _luminanceRangeFilter;
    
    float _blurSigma;
    float _saturation;
    float _rangeReductionFactor;
    float _downSampling;
};


NS_GI_END

#endif /* IOSBlurFilter_hpp */
