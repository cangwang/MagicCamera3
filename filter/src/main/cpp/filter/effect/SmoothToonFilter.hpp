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

#ifndef SmoothToonFilter_hpp
#define SmoothToonFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "GaussianBlurFilter.hpp"
#include "ToonFilter.hpp"

NS_GI_BEGIN

class SmoothToonFilter : public FilterGroup {
public:
    virtual ~SmoothToonFilter();
    
    static SmoothToonFilter* create();
    bool init();
    
    void setBlurRadius(int blurRadius);
    void setToonThreshold(float toonThreshold);
    void setToonQuantizationLevels(float toonQuantizationLevels);
    
protected:
    SmoothToonFilter();
    
private:
    GaussianBlurFilter* _gaussianBlurFilter;
    ToonFilter* _toonFilter;
    
    float _blurRadius;
    float _toonThreshold;
    float _toonQuantizationLevels;
};


NS_GI_END

#endif /* SmoothToonFilter_hpp */
