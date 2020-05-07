#include "Framebuffer.hpp"
#include "cassert"
#include <algorithm>
#include "Context.hpp"
#include "util.h"

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN

//TODO:解析
std::vector<Framebuffer*> Framebuffer::_framebuffers;

TextureAttributes Framebuffer::defaultTextureAttributes = {
            .minFilter = GL_LINEAR,
            .magFilter = GL_LINEAR,
            .wrapS = GL_CLAMP_TO_EDGE,
            .wrapT = GL_CLAMP_TO_EDGE,
            .internalFormat = GL_RGBA,
            .format = GL_RGBA,
            .type = GL_UNSIGNED_BYTE
};

Framebuffer::Framebuffer(int width, int height, bool onlyGenerateTexture,
                         const GPUImage::TextureAttributes textureAttributes)
                         :_texture(static_cast<GLuint>(-1))
                         ,_framebuffer(static_cast<GLuint>(-1)) {
    _width = width;
    _height = height;
    _textureAttributes = textureAttributes;
    _hasFB = !onlyGenerateTexture;

    if (_hasFB) {

    } else {

    }

    _framebuffers.push_back(this);
}

Framebuffer::~Framebuffer() {

    auto itr = std::find(_framebuffers.begin(),
            _framebuffers.end(),this);
    //释放帧缓冲
    if (itr != _framebuffers.end()) {
        _framebuffers.erase(itr);
    }

    bool bDeleteTex = (_texture != -1);
    bool bDeleteFB = (_framebuffer != -1);

    //自动推断类型
    for (auto const& framebuffer : _framebuffers) {
        if (bDeleteTex) {
            if (_texture == framebuffer->getTexture()){
                bDeleteTex = false;
            }
        }

        if (bDeleteFB) {
            if (framebuffer->hasFramebuffer()
                && _framebuffer == framebuffer->getFramebuffer()){
                bDeleteFB = false;
            }
        }
        if (!bDeleteTex && !bDeleteFB) break;
    }

    if (bDeleteTex) {
        CHECK_GL(glDeleteTextures(1, &_texture));
        _texture = static_cast<GLuint>(-1);
    }

    if (bDeleteFB) {
        CHECK_GL(glDeleteFramebuffers(1, &_framebuffer));
        _framebuffer = static_cast<GLuint>(-1);
    }
}

//释放
void Framebuffer::release(bool returnToCache) {
    if (returnToCache) {
        assert(_referenceCount > 0);
        --_referenceCount;
        if (_referenceCount == 0) {

        }
    } else {
        Ref::release();
    }
}

//激活framebuffer
void Framebuffer::active() {
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer));
    CHECK_GL(glViewport(0, 0, _width, _height));
}

//关闭framebuffer
void Framebuffer::inactive() {
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

//合成纹理
void Framebuffer::_generateTexture() {
    CHECK_GL(glGenTextures(1, &_texture));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, _texture));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureAttributes.minFilter));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureAttributes.magFilter));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _textureAttributes.wrapS));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _textureAttributes.wrapT));

    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

//合成FrameBuffer
void Framebuffer::_generateFramebuffer() {
    CHECK_GL(glGenFramebuffers(1, &_framebuffer));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer));
    _generateTexture();
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, _texture));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, _textureAttributes.internalFormat, _width, _height,
            0, _textureAttributes.format, _textureAttributes.type, 0));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            _texture, 0 ));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

NS_GI_END