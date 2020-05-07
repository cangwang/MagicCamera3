#ifndef Ref_hpp
#define Ref_hpp

#include "macros.hpp"
/**
 * cangwang 2020.3.5
 * 引用计数类
 */

NS_GI_BEGIN
class Ref {
public:
    virtual ~Ref();

    void retain();
    virtual void release();
    void resetReferenceCount();
    unsigned int getReferenceCount() const;

protected:
    unsigned int _referenceCount;
    Ref();
};

NS_GI_END

#endif