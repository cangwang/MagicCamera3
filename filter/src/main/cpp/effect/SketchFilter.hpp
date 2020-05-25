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

#ifndef SketchFilter_hpp
#define SketchFilter_hpp

#include "../source/macros.h"
#include "FilterGroup.hpp"
#include "GrayscaleFilter.hpp"
#include "NearbySampling3x3Filter.hpp"

NS_GI_BEGIN

// Sketch filter is just the Sobel edge detection filter with the colors inverted.

class _SketchFilter;

class SketchFilter : public FilterGroup {
public:
    static SketchFilter* create();
    bool init();
    
protected:
    SketchFilter();
    ~SketchFilter();
    
    GrayscaleFilter* _grayscaleFilter;
    _SketchFilter* _sketchFilter;
    
    float _edgeStrength;
};

class _SketchFilter : public NearbySampling3x3Filter {
public:
    static _SketchFilter* create();
    bool init();
    virtual bool proceed(bool bUpdateTargets = true) override;
    
    void setEdgeStrength(float edgeStrength);
    
protected:
    _SketchFilter() {};
    
    float _edgeStrength;
};

NS_GI_END

#endif /* SketchFilter_hpp */
