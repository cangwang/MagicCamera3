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

#include "BeautifyFilter.hpp"


NS_GI_BEGIN


// Beautify shader code is adapted from guikz's beautify filter demo
// reference :
// http://www.csie.ntu.edu.tw/~fuh/personal/FaceBeautificationandColorEnhancement.A2-1-0040.pdf
// http://m.blog.csdn.net/article/details?id=50496969
const std::string kBeautifyCombinationFragmentShaderString = SHADER_STRING
(
 varying highp vec2 vTexCoord;
 varying highp vec2 vTexCoord1;
 varying highp vec2 vTexCoord2;
 
 uniform sampler2D colorMap;
 uniform sampler2D colorMap1;
 uniform sampler2D colorMap2;
 uniform mediump float smoothDegree;
 
 void main()
 {
     highp vec4 bilateral = texture2D(colorMap, vTexCoord);
     highp vec4 canny = texture2D(colorMap1, vTexCoord1);
     
     highp vec4 origin = texture2D(colorMap2,vTexCoord2);
     highp vec4 smooth;
     lowp float r = origin.r;
     lowp float g = origin.g;
     lowp float b = origin.b;
     
     if (canny.r < 0.2 && r > 0.3725 && g > 0.1568 && b > 0.0784 && r > b && (max(max(r, g), b) - min(min(r, g), b)) > 0.0588 && abs(r-g) > 0.0588) {
         smooth = (1.0 - smoothDegree) * (origin - bilateral) + bilateral;
     }
     else {
         smooth = origin;
     }
     
     smooth.r = log(1.0 + 0.2 * smooth.r)/log(1.2);
     smooth.g = log(1.0 + 0.2 * smooth.g)/log(1.2);
     smooth.b = log(1.0 + 0.2 * smooth.b)/log(1.2);
     gl_FragColor = smooth;
     //gl_FragColor = origin;
 }
 );

class CombinationFilter : public Filter {
public:
    static CombinationFilter* create() {
        CombinationFilter* ret = new (std::nothrow) CombinationFilter();
        if (!ret || !ret->init()) {
            delete ret;
            ret = 0;
        }
        return ret;
    }
    
    bool init() {
        if (!Filter::initWithFragmentShaderString(kBeautifyCombinationFragmentShaderString, 3)) {
            return false;
        }
        _intensity = 0.5;
        
        return true;
    }
    
    virtual bool proceed(bool bUpdateTargets = true) override {
        _filterProgram->setUniformValue("smoothDegree", _intensity);
        return Filter::proceed(bUpdateTargets);
    }
    
protected:
    CombinationFilter() {};
    
private:
    float _intensity;
};

REGISTER_FILTER_CLASS(BeautifyFilter)

BeautifyFilter::BeautifyFilter()
:_bilateralFilter(0)
,_cannyEdgeDetectionFilter(0)
,_combinationFilter(0)
,_hsbFilter(0)
{
}

BeautifyFilter::~BeautifyFilter() {
    if (_bilateralFilter) {
        _bilateralFilter->release();
        _bilateralFilter = 0;
    }
    if (_cannyEdgeDetectionFilter) {
        _cannyEdgeDetectionFilter->release();
        _cannyEdgeDetectionFilter = 0;
    }
    if (_combinationFilter) {
        _combinationFilter->release();
        _combinationFilter = 0;
    }
    if (_hsbFilter) {
        _hsbFilter->release();
        _hsbFilter = 0;
    }
}

BeautifyFilter* BeautifyFilter::create() {
    BeautifyFilter* ret = new (std::nothrow) BeautifyFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool BeautifyFilter::init() {
    if (!FilterGroup::init()) {
        return false;
    }
    
    // 1. face smoothing
    _bilateralFilter = BilateralFilter::create();
    _bilateralFilter->setDistanceNormalizationFactor(4.0);
    addFilter(_bilateralFilter);
    
    // 2. edge detection
    _cannyEdgeDetectionFilter = CannyEdgeDetectionFilter::create();
    addFilter(_cannyEdgeDetectionFilter);
    
    // 3.combination bilateral, edge detection and origin
    _combinationFilter = CombinationFilter::create();
    addFilter(_combinationFilter);
    
    _bilateralFilter->addTarget(_combinationFilter);
    _cannyEdgeDetectionFilter->addTarget(_combinationFilter);

    // 4. adjust hsb
    _hsbFilter = HSBFilter::create();
    _hsbFilter->adjustBrightness(1.1);
    _hsbFilter->adjustSaturation(1.1);
    _combinationFilter->addTarget(_hsbFilter);
     
    setTerminalFilter(_hsbFilter);
    
    return true;
}


bool BeautifyFilter::proceed(bool bUpdateTargets/* = true*/) {
    return FilterGroup::proceed(bUpdateTargets);
}


void BeautifyFilter::setInputFramebuffer(Framebuffer* framebuffer, RotationMode rotationMode/* = NoRotation*/, int texIdx/* = 0*/) {
    for (auto& filter : _filters) {
        if (filter == _combinationFilter)
            texIdx = 2;
        filter->setInputFramebuffer(framebuffer, rotationMode, texIdx);
    }
}

NS_GI_END
