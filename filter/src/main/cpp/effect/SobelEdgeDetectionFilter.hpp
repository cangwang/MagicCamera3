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

#ifndef SobelEdgeDetectionFilter_hpp
#define SobelEdgeDetectionFilter_hpp

#include "../source/macros.h"
#include "FilterGroup.hpp"
#include "GrayscaleFilter.hpp"
#include "NearbySampling3x3Filter.hpp"

NS_GI_BEGIN

class _SobelEdgeDetectionFilter;

class SobelEdgeDetectionFilter : public FilterGroup {
public:
    static SobelEdgeDetectionFilter* create();
    bool init();
    
protected:
    SobelEdgeDetectionFilter();
    ~SobelEdgeDetectionFilter();
    
    GrayscaleFilter* _grayscaleFilter;
    _SobelEdgeDetectionFilter* _sobelEdgeDetectionFilter;
    
    float _edgeStrength;
};

class _SobelEdgeDetectionFilter : public NearbySampling3x3Filter {
public:
    static _SobelEdgeDetectionFilter* create();
    bool init();
    virtual bool proceed(bool bUpdateTargets = true) override;
    
    void setEdgeStrength(float edgeStrength);
    
protected:
    _SobelEdgeDetectionFilter() {};
    
    float _edgeStrength;
};

NS_GI_END

#endif /* SobelEdgeDetectionFilter_hpp */
