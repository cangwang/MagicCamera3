#ifndef Target_hpp
#define Target_hpp

#include "macros.hpp"
#include "Framebuffer.hpp"
#include <map>

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN

enum RotationMode {
    NoRotation = 0,
    RotateLeft,
    RotateRight,
    FlipVertical,
    FlipHorizontal,
    RotateRightFlipVertical,
    RotateRightFlipHorizontal,
    Rotate180
};

class Target: public virtual Ref {
public:
    Target(int inputNumber = 1);
    virtual ~Target();
    virtual void setInputFramebuffer(Framebuffer* framebuffer,
            RotationMode rotationMode = NoRotation,
            int texIdx = 0);
    virtual bool isPrepared() const;
    virtual void unPrepare();
    virtual void update(float frameTime){};
    virtual int getNextAvailableTextureIndex() const;

protected:
    struct InputFrameBufferInfo {
        Framebuffer* framebuffer;
        RotationMode rotationMode;
        int texIndex;
        bool ignoreForPrepare;
    };

    std::map<int, InputFrameBufferInfo> _inputFramebuffers;
    int _inputNum;
};

NS_GI_END

#endif