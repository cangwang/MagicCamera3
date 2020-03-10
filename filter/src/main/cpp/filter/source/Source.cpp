#include "Source.hpp"
#include "util.h"
#include "Context.hpp"

#if PLATFORM == PLATFORM_IOS
#include "IOSTarget.hpp"
#endif

/**
 * cangwang 2020.3.5
 */

NS_GI_BEGIN
Source::Source()
:_framebuffer(0)
,_outputRotation(RotationMode::NoRotation)
,_framebufferScale(1.0) {

}

Source::~Source() {
    if (_framebuffer != 0) {
        _framebuffer->release();
        _framebuffer = 0;
    }

    removeAllTargets();
}

Source* Source::addTarget(Target *target) {
    int targetTexIdx = target->getNextAvailableTextureIndex();
    return addTarget(target, targetTexIdx);
}

Source* Source::addTarget(Target *target, int texIdx) {
    if (!hasTarget(target)) {
        _targets[target] = texIdx;
        target->setInputFramebuffer(_framebuffer, RotationMode::NoRotation, texIdx);
        target->retain();
    }
    return dynamic_cast<Source*>(target);
}

#if PLATFORM == PLATFORM_IOS
Source* Source::addTarget(id<GPUImageTarget> target) {
    return 0;
}
#endif

bool Source::hasTarget(const GPUImage::Target* target) const {
    return _targets.find(const_cast<Target *>(target)) != _targets.end();
}

void Source::removeTarget(GPUImage::Target *target) {
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
    for(auto const& target : _targets) {
        Ref* ref = (Ref*)(target.first);
        if (ref) {
            ref->release();
        }
    }
    _targets.clear();
}

bool Source::proceed(bool bUpdateTargets) {
    if (bUpdateTargets){
        updateTargets(0);
    }
    return true;
}

void Source::updateTargets(float frameTime) {
    for(auto& it :_targets) {
        Target* target = it.first;
        target->setInputFramebuffer(_framebuffer, _outputRotation, _targets[target]);
        if (target->isPrepared()) {
            target->update(frameTime);
            target->unPrepare();
        }
    }
}

unsigned char* Source::captureAProcessedFrameData(Filter *upToFilter, int width, int height) {
    if (Context::getInstance()->isCapturingFrame) return nullptr;

    if (width <= 0 || height <= 0) {
        if (!_framebuffer) return nullptr;
        width = getRotatedFramebufferWidth();
        height = getRotatedFramebufferHeight();
    }

    Context::getInstance()->isCapturingFrame = true;
    Context::getInstance()->captureWidth = width;
    Context::getInstance()->captureHeight = height;
    Context::getInstance()->captureUpToFilter = upToFilter;

    proceed(true);
    unsigned char* processedFrameData = Context::getInstance()->capturedFrameData;

    Context::getInstance()->capturedFrameData = nullptr;
    Context::getInstance()->captureWidth = 0;
    Context::getInstance()->captureHeight = 0;
    Context::getInstance()->isCapturingFrame = false;

    return processedFrameData;
}

void Source::setFrameBuffer(GPUImage::Framebuffer *fb, GPUImage::RotationMode outputRotation) {
    if (_framebuffer != fb &&_framebuffer != nullptr) {
        _framebuffer->release();
        _framebuffer = nullptr;
    }

    _framebuffer = fb;
    if (_framebuffer) {
        _framebuffer->retain();
    }

    _outputRotation = outputRotation;
}

int Source::getRotatedFramebufferWidth() const {
    if (_framebuffer) {
        if (rotationSwapsSize(_outputRotation)) {
            return _framebuffer->getHeight();
        } else {
            return _framebuffer->getWidth();
        }
    } else {
        return 0;
    }
}

int Source::getRotatedFramebufferHeight() const {
    if (_framebuffer) {
        if (rotationSwapsSize(_outputRotation)) {
            return _framebuffer->getWidth();
        } else {
            return _framebuffer->getHeight();
        }
    } else {
        return 0;
    }
}

Framebuffer* Source::getFramebuffer() const {
    return _framebuffer;
}

void Source::releaseFramebuffer(bool returnToCache) {
    if(_framebuffer != 0) {
        _framebuffer->release(returnToCache);
        _framebuffer = nullptr;
    }
}

NS_GI_END