#include "Ref.hpp"
#include <cassert>

/**
 * cangwang 2020.3.5
 * 引用计数类
 */

NS_GI_BEGIN
Ref::Ref():_referenceCount(1) {

}

Ref::~Ref() {

}

void Ref::retain() {
    assert(_referenceCount > 0);
    ++_referenceCount;
}

void Ref::release() {
    assert(_referenceCount > 0);
    --_referenceCount;
    if (_referenceCount == 0) {
        delete this;
    }
}

void Ref::resetReferenceCount() {
    _referenceCount = 1;
}

unsigned int Ref::getReferenceCount() const {
    return _referenceCount;
}

NS_GI_END