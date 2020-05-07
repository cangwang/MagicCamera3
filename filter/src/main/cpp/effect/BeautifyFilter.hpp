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

#ifndef BeautifyFilter_hpp
#define BeautifyFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "BilateralFilter.hpp"
#include "CannyEdgeDetectionFilter.hpp"
#include "ColorInvertFilter.hpp"
#include "GrayscaleFilter.hpp"
#include "HSBFilter.hpp"

NS_GI_BEGIN

    class CombinationFilter;

    class BeautifyFilter : public FilterGroup {
    public:
        static BeautifyFilter* create();
        bool init();
        bool proceed(bool bUpdateTargets = true) override;

        virtual void setInputFramebuffer(Framebuffer* framebuffer, RotationMode rotationMode = NoRotation, int texIdx = 0) override;

    protected:
        BeautifyFilter();
        ~BeautifyFilter();

        BilateralFilter* _bilateralFilter;
        CannyEdgeDetectionFilter* _cannyEdgeDetectionFilter;
        CombinationFilter* _combinationFilter;
        HSBFilter* _hsbFilter;
    };

NS_GI_END

#endif /* BeautifyFilter_hpp */
