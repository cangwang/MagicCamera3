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

#include "Framebuffer.hpp"
#include <assert.h>
#include <algorithm>
#include "Context.hpp"
#include "util.h"


NS_GI_BEGIN

std::vector<Framebuffer*> Framebuffer::_framebuffers;

TextureAttributes Framebuffer::defaultTextureAttribures = {
    .minFilter = GL_LINEAR,
    .magFilter = GL_LINEAR,
    .wrapS = GL_CLAMP_TO_EDGE,
    .wrapT = GL_CLAMP_TO_EDGE,
    .internalFormat = GL_RGBA,
    .format = GL_RGBA,
    .type = GL_UNSIGNED_BYTE
};

Framebuffer::Framebuffer(int width, int height, bool onlyGenerateTexture/* = false*/, const TextureAttributes textureAttributes/* = defaultTextureAttribures*/)
:_texture(-1)
,_framebuffer(-1)
{
    _width = width;
    _height = height;
    _textureAttributes = textureAttributes;
    _hasFB = !onlyGenerateTexture;
    
    if (_hasFB) {
        _generateFramebuffer();
    } else {
        _generateTexture();
    }

    _framebuffers.push_back(this);
}

Framebuffer::~Framebuffer() {
    // todo
    std::vector<Framebuffer*>::iterator itr = std::find(_framebuffers.begin(), _framebuffers.end(), this);
    if (itr != _framebuffers.end()) {
        _framebuffers.erase(itr);
    }

    bool bDeleteTex = (_texture != -1);
    bool bDeleteFB = (_framebuffer != -1);

    for (auto const& framebuffer : _framebuffers ) {
        if (bDeleteTex) {
            if (_texture == framebuffer->getTexture()) {
                bDeleteTex = false;
            }
        }

        if (bDeleteFB) {
            if (framebuffer->hasFramebuffer() && _framebuffer == framebuffer->getFramebuffer()) {
                bDeleteFB = false;
            }
        }
    }

    if (bDeleteTex) {
        CHECK_GL(glDeleteTextures(1, &_texture));
        _texture = -1;
    }
    if (bDeleteFB) {
        CHECK_GL(glDeleteFramebuffers(1, &_framebuffer));
        _framebuffer = -1;
    }
}

void Framebuffer::release(bool returnToCache/* = true*/) {
    if (returnToCache) {
        assert(_referenceCount > 0);
        --_referenceCount;
        if (_referenceCount == 0) {
            Context::getInstance()->getFramebufferCache()->returnFramebuffer(this);
        }
    } else {
        Ref::release();
    }
}

void Framebuffer::active() {
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer));
    CHECK_GL(glViewport(0, 0, _width, _height));
}

void Framebuffer::inactive() {
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::_generateTexture() {
    CHECK_GL(glGenTextures(1, &_texture));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, _texture));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _textureAttributes.minFilter));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _textureAttributes.magFilter));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _textureAttributes.wrapS));
    CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _textureAttributes.wrapT));

    // TODO: Handle mipmaps
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Framebuffer::_generateFramebuffer() {
    CHECK_GL(glGenFramebuffers(1, &_framebuffer));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer));
    _generateTexture();
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, _texture));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, _textureAttributes.internalFormat, _width, _height, 0, _textureAttributes.format, _textureAttributes.type, 0));
    CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
    CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


NS_GI_END
