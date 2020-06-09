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

#ifndef Source_hpp
#define Source_hpp

#include "macros.h"
#include "Target.hpp"
#include "Ref.hpp"
#include <map>
#include <functional>

#if PLATFORM == PLATFORM_IOS
#import "GPUImageTarget.h"
#endif

NS_GI_BEGIN

class Filter;
class Source : public virtual Ref {
public:
    Source();
    virtual ~Source();
    virtual Source* addTarget(Target* target);
    virtual Source* addTarget(Target* target, int texIdx);
#if PLATFORM == PLATFORM_IOS
    virtual Source* addTarget(id<GPUImageTarget> target);
#endif
    virtual void removeTarget(Target* target);
    virtual void removeAllTargets();
    virtual bool hasTarget(const Target* target) const;
    virtual std::map<Target*, int>& getTargets() { return _targets; };

    virtual void setFramebuffer(Framebuffer* fb, RotationMode outputRotation = RotationMode::NoRotation);
    virtual Framebuffer* getFramebuffer() const;
    virtual void releaseFramebuffer(bool returnToCache = true);
    
    void setFramebufferScale(float framebufferScale) { _framebufferScale = framebufferScale; }
    int getRotatedFramebufferWidth() const;
    int getRotatedFramebufferHeight() const;
    
    virtual bool proceed(bool bUpdateTargets = true);
    virtual void setTouchLocation(float x, float y);
    virtual void updateTargets(float frameTime);

    virtual unsigned char* captureAProcessedFrameData(Filter* upToFilter, int width = 0, int height = 0);
    
protected:
    Framebuffer* _framebuffer;
    RotationMode _outputRotation;
    std::map<Target*, int> _targets;
    float _framebufferScale;
};


NS_GI_END

#endif /* Source_hpp */
