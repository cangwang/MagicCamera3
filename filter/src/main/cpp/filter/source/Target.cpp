#include "Target.hpp"
#include "util.h"

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN
Target::Target(int inputNumber)
:_inputNum(inputNumber){

}

Target::~Target() {
    for(std::map<int, InputFrameBufferInfo>::iterator it = _inputFramebuffers.begin();
    it != _inputFramebuffers.end(); ++it) {
        if (it->second.framebuffer) {
            it->second.framebuffer->release();
            it->second.framebuffer = 0;
        }
    }
    _inputFramebuffers.clear();
}

void Target::setInputFramebuffer(GPUImage::Framebuffer *framebuffer,
                                 GPUImage::RotationMode rotationMode, int texIdx) {
    InputFrameBufferInfo inputFrameBufferInfo;
    inputFrameBufferInfo.framebuffer = framebuffer;
    inputFrameBufferInfo.rotationMode = rotationMode;
    inputFrameBufferInfo.texIndex = texIdx;
    inputFrameBufferInfo.ignoreForPrepare = false;

    if (_inputFramebuffers.find(texIdx) != _inputFramebuffers.end()
        && _inputFramebuffers[texIdx].framebuffer){
        _inputFramebuffers[texIdx].framebuffer->release();
        _inputFramebuffers[texIdx].framebuffer = 0;
    }

    _inputFramebuffers[texIdx] = inputFrameBufferInfo;
    if(_inputFramebuffers[texIdx].framebuffer) {
        _inputFramebuffers[texIdx].framebuffer->retain();
    }
}

int Target::getNextAvailableTextureIndex() const {
    for (int i = 0; i < _inputNum; ++i) {
        if (_inputFramebuffers.find(i) == _inputFramebuffers.end()) {
            return i;
        }
    }
    return _inputNum - 1;
}

bool Target::isPrepared() const {
    int preparedNum = 0;
    int ignoreForPrepareNum = 0;
    for (std::map<int, InputFrameBufferInfo>::const_iterator it = _inputFramebuffers.begin();
        it != _inputFramebuffers.end(); ++it) {
        if (it->second.ignoreForPrepare) {
            ignoreForPrepareNum++;
        } else if (it->second.framebuffer) {
            preparedNum++;
        }
    }

    if (ignoreForPrepareNum + preparedNum>= _inputNum) {
        return true;
    } else {
        return false;
    }
}

void Target::unPrepare() {
    for (auto it = _inputFramebuffers.begin();
         it != _inputFramebuffers.end(); ++it) {
        if (!it->second.ignoreForPrepare) {
            if (it->second.framebuffer) {
                it->second.framebuffer->release();
                it->second.framebuffer = 0;
            }
        }
    }
}

NS_GI_END