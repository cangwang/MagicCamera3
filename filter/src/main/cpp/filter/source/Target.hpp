#ifndef Target_hpp
#define Target_hpp

#include "marcos.hpp"
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

};

NS_GI_END

#endif