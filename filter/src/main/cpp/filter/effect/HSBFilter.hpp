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

#ifndef HSBFilter_hpp
#define HSBFilter_hpp

#include "../source/macros.hpp"
#include "ColorMatrixFilter.hpp"

NS_GI_BEGIN


class HSBFilter : public ColorMatrixFilter {
public:
    static HSBFilter* create();
    bool init();

    /** Reset the filter to have no transformations.
     */
    void reset();

    /** Add a hue rotation to the filter.
     The hue rotation is in the range [-360, 360] with 0 being no-change.
     Note that this adjustment is additive, so use the reset method if you need to.
     */
    void rotateHue(float h);

    /** Add a saturation adjustment to the filter.
     The saturation adjustment is in the range [0.0, 2.0] with 1.0 being no-change.
     Note that this adjustment is additive, so use the reset method if you need to.
     */
    void adjustSaturation(float s);

    /** Add a brightness adjustment to the filter.
     The brightness adjustment is in the range [0.0, 2.0] with 1.0 being no-change.
     Note that this adjustment is additive, so use the reset method if you need to.
     */
    void adjustBrightness(float b);

protected:
    HSBFilter() {};
};


NS_GI_END

#endif /* HSBFilter_hpp */
