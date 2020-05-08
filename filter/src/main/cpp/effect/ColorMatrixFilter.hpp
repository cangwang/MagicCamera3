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

#ifndef ColorMatrixFilter_hpp
#define ColorMatrixFilter_hpp

#include "../source/macros.h"
#include "Filter.hpp"
#include "../source/math.hpp"

NS_GI_BEGIN


class ColorMatrixFilter : public Filter {
public:
    static ColorMatrixFilter* create();
    bool init();
    
    virtual bool proceed(bool bUpdateTargets = true) override;
    
    void setIntensity(float intensity) { _intensity = intensity; }
    void setColorMatrix(Matrix4 colorMatrix) { _colorMatrix = colorMatrix; }
    
protected:
    ColorMatrixFilter();
    
    float _intensity;
    Matrix4 _colorMatrix;
};


NS_GI_END

#endif /* ColorMatrixFilter_hpp */
