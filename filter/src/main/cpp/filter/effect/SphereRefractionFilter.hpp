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

#ifndef SphereRefractionFilter_hpp
#define SphereRefractionFilter_hpp

#include "../source/macros.hpp"
#include "Filter.hpp"

NS_GI_BEGIN

class SphereRefractionFilter : public Filter {
public:
    static SphereRefractionFilter* create();
    bool init();
    virtual bool proceed(bool bUpdateTargets = true) override;

    void setPositionX(float x);
    void setPositionY(float y);
    void setRadius(float radius);
    void setRefractiveIndex(float refractiveIndex);
    
protected:
    SphereRefractionFilter() {};
    
    // The position about which to apply the distortion, with a default of (0.5, 0.5)
    Vector2 _position;

    // The radius of the distortion, ranging from 0.0 to 1.0, with a default of 0.25
    float _radius;
    
    // The index of refraction for the sphere, with a default of 0.71
    float _refractiveIndex;
    
};

NS_GI_END

#endif /* SphereRefractionFilter_hpp */
