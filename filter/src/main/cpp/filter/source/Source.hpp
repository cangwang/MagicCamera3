#ifndef Source_hpp
#define Source_hpp

#include "macros.hpp"
#include "Target.hpp"
#include <map>
#include <functional>

#if PLATFORM == PLATFORM_IOS
#import "GPUImageTarget.h"
#endif

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN
class Filter;
class Source: public virtual Ref {
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
    virtual std::map<Target*, int>& getTargets() {
        return _targets;
    };
    virtual void setFrameBuffer(Framebuffer* fb,
            RotationMode outputRotation = RotationMode::NoRotation);
    virtual Framebuffer* getFramebuffer() const ;
    virtual void releaseFramebuffer(bool returnToCache = true);

    void setFramebufferScale(float framebufferScale) {
        _framebufferScale = framebufferScale;
    };

    int getRotatedFramebufferWidth() const ;
    int getRotatedFramebufferHeight() const ;

    virtual bool proceed(bool bUpdateTargets = true);
    virtual void updateTargets(float frameTime);
    virtual unsigned char* captureAProcessedFrameData(Filter* upToFilter,
            int width = 0, int height = 0);

protected:
    Framebuffer* _framebuffer;
    RotationMode _outputRotation;
    std::map<Target*, int> _targets;
    float _framebufferScale;
};


NS_GI_END

#endif