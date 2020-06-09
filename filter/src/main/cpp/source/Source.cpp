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

#include "Source.hpp"
#include "util.h"
#include "Context.hpp"

#if PLATFORM == PLATFORM_IOS
#include "IOSTarget.hpp"
#endif

NS_GI_BEGIN


Source::Source()
:_framebuffer(0)
,_outputRotation(RotationMode::NoRotation)
,_framebufferScale(1.0)
{
    
}

Source::~Source() {
    if (_framebuffer != 0) {
        _framebuffer->release();
        _framebuffer = 0;
    }

    removeAllTargets();
}

Source* Source::addTarget(Target* target) {
    int targetTexIdx = target->getNextAvailableTextureIndex();
    return addTarget(target, targetTexIdx);
}

Source* Source::addTarget(Target* target, int texIdx) {
    if (!hasTarget(target)) {
        _targets[target] = texIdx;
        target->setInputFramebuffer(_framebuffer, RotationMode::NoRotation, texIdx);
//        Ref *ref = dynamic_cast<Ref *>(target);
//        if (ref) {
//            ref->retain();
//        }
        target->retain();
    }
    return dynamic_cast<Source*>(target);
}

#if PLATFORM == PLATFORM_IOS
Source* Source::addTarget(id<GPUImageTarget> target) {
    IOSTarget* iosTarget = new IOSTarget(target);
    addTarget(iosTarget);
    iosTarget->release();
    return 0;
}
#endif

bool Source::hasTarget(const Target* target) const {
    return _targets.find(const_cast<Target *>(target)) != _targets.end();
}

void Source::removeTarget(Target* target) {
    auto itr = _targets.find(target);
    if (itr != _targets.end()) {
        Ref* ref = (Ref*)(itr->first);
        if (ref) {
            ref->release();
        }
        _targets.erase(itr);
    }
}

void Source::removeAllTargets() {
    for (auto const& target : _targets ) {
        Ref* ref = (Ref*)(target.first);
        if (ref) {
            ref->release();
        }
    }
    _targets.clear();
}

bool Source::proceed(bool bUpdateTargets/* = true*/) {
    if (bUpdateTargets)
        updateTargets(0);
    return true;
}

void Source::setTouchLocation(float x, float y) {

}

void Source::updateTargets(float frameTime) {
    for(auto& it : _targets){
        Target* target = it.first;
        target->setInputFramebuffer(_framebuffer, _outputRotation, _targets[target]);
        if (target->isPrepared()) {
            target->update(frameTime);
            target->unPrepare();
        }
    }
}

unsigned char* Source::captureAProcessedFrameData(Filter* upToFilter, int width/* = 0*/, int height/* = 0*/) {
    if (Context::getInstance()->isCapturingFrame) return 0 ;

    if (width <= 0 || height <= 0) {
        if (!_framebuffer) return 0;
        width = getRotatedFramebufferWidth();
        height = getRotatedFramebufferHeight();
    }
    
    Context::getInstance()->isCapturingFrame = true;
    Context::getInstance()->captureWidth = width;
    Context::getInstance()->captureHeight = height;
    Context::getInstance()->captureUpToFilter = upToFilter;

    proceed(true);
    unsigned char* processedFrameData = Context::getInstance()->capturedFrameData;

    Context::getInstance()->capturedFrameData = 0;
    Context::getInstance()->captureWidth = 0;
    Context::getInstance()->captureHeight = 0;
    Context::getInstance()->isCapturingFrame = false;
    
    return processedFrameData;
}

void Source::setFramebuffer(Framebuffer* fb, RotationMode outputRotation/* = RotationMode::NoRotation*/) {
    if (_framebuffer != fb && _framebuffer != 0) {
        _framebuffer->release();
        _framebuffer = 0;
    }
    _framebuffer = fb;
    if (_framebuffer)
        _framebuffer->retain();
    _outputRotation = outputRotation;
}

int Source::getRotatedFramebufferWidth() const {
    if (_framebuffer)
        if (rotationSwapsSize(_outputRotation))
            return _framebuffer->getHeight();
        else
            return _framebuffer->getWidth();
    else
        return 0;
}

int Source::getRotatedFramebufferHeight() const {
    if (_framebuffer)
        if (rotationSwapsSize(_outputRotation))
            return _framebuffer->getWidth();
        else
            return _framebuffer->getHeight();
    else
        return 0;
}

Framebuffer* Source::getFramebuffer() const {
    return _framebuffer;
}

void Source::releaseFramebuffer(bool returnToCache/* = true*/) {
    if (_framebuffer != 0) {
        _framebuffer->release(returnToCache);
        _framebuffer = 0;
    }
}

NS_GI_END
