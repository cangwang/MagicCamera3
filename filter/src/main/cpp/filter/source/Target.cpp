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
    for(auto & _inputFramebuffer : _inputFramebuffers) {
        if (_inputFramebuffer.second.framebuffer) {
            _inputFramebuffer.second.framebuffer->release();
            _inputFramebuffer.second.framebuffer = nullptr;
        }
    }
    _inputFramebuffers.clear();
}

void Target::setInputFramebuffer(GPUImage::Framebuffer *framebuffer,
                                 GPUImage::RotationMode rotationMode, int texIdx) {
    InputFrameBufferInfo inputFrameBufferInfo{};
    inputFrameBufferInfo.framebuffer = framebuffer;
    inputFrameBufferInfo.rotationMode = rotationMode;
    inputFrameBufferInfo.texIndex = texIdx;
    inputFrameBufferInfo.ignoreForPrepare = false;

    if (_inputFramebuffers.find(texIdx) != _inputFramebuffers.end()
        && _inputFramebuffers[texIdx].framebuffer){
        _inputFramebuffers[texIdx].framebuffer->release();
        _inputFramebuffers[texIdx].framebuffer = nullptr;
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
    for (const auto & _inputFramebuffer : _inputFramebuffers) {
        if (_inputFramebuffer.second.ignoreForPrepare) {
            ignoreForPrepareNum++;
        } else if (_inputFramebuffer.second.framebuffer) {
            preparedNum++;
        }
    }

    return ignoreForPrepareNum + preparedNum >= _inputNum;
}

void Target::unPrepare() {
    for (auto & _inputFramebuffer : _inputFramebuffers) {
        if (!_inputFramebuffer.second.ignoreForPrepare) {
            if (_inputFramebuffer.second.framebuffer) {
                _inputFramebuffer.second.framebuffer->release();
                _inputFramebuffer.second.framebuffer = nullptr;
            }
        }
    }
}

NS_GI_END