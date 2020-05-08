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

#include "FramebufferCache.hpp"
#include "util.h"

NS_GI_BEGIN


FramebufferCache::FramebufferCache()
{
}

FramebufferCache::~FramebufferCache() {
    purge();
}

Framebuffer* FramebufferCache::fetchFramebuffer(int width, int height, bool onlyTexture/* = false*/, const TextureAttributes textureAttributes/* = defaultTextureAttribure*/) {
    
    Framebuffer* framebufferFromCache = 0;
    std::string lookupHash = _getHash(width, height, onlyTexture, textureAttributes);
    int numberOfMatchingFramebuffers = 0;
    if(_framebufferTypeCounts.find(lookupHash) != _framebufferTypeCounts.end()) {
        numberOfMatchingFramebuffers = _framebufferTypeCounts[lookupHash];
    }
    if (numberOfMatchingFramebuffers < 1) {
        framebufferFromCache = new Framebuffer(width, height, onlyTexture, textureAttributes);
    } else {
        int curFramebufferId = numberOfMatchingFramebuffers - 1;
        while (!framebufferFromCache && curFramebufferId >= 0) {
            std::string framebufferHash = str_format("%s-%ld", lookupHash.c_str(), curFramebufferId);
            if (_framebuffers.find(framebufferHash) != _framebuffers.end()) {
                framebufferFromCache = _framebuffers[framebufferHash];
                _framebuffers.erase(framebufferHash);
            } else
                framebufferFromCache = 0;
            curFramebufferId--;
        }
        curFramebufferId++;
        _framebufferTypeCounts[lookupHash] = curFramebufferId;
        
        if (!framebufferFromCache) {
            framebufferFromCache = new Framebuffer(width, height, onlyTexture, textureAttributes);
        }
    }
    
    // make sure this framebuffer is not referenced by others
    framebufferFromCache->resetRefenceCount();
    return framebufferFromCache;
}

void FramebufferCache::returnFramebuffer(Framebuffer* framebuffer) {
    if (framebuffer == 0) return;
    int width = framebuffer->getWidth();
    int height = framebuffer->getHeight();
    const TextureAttributes& textureAttributes = framebuffer->getTextureAttributes();
    std::string lookupHash = _getHash(width, height, !framebuffer->hasFramebuffer(), textureAttributes);
    int numberOfMatchingFramebuffers = 0;
    if(_framebufferTypeCounts.find(lookupHash) != _framebufferTypeCounts.end()) {
        numberOfMatchingFramebuffers = _framebufferTypeCounts[lookupHash];
    }
    std::string framebufferHash = str_format("%s-%ld", lookupHash.c_str(), numberOfMatchingFramebuffers);
    _framebuffers[framebufferHash] = framebuffer;
    _framebufferTypeCounts[lookupHash] = numberOfMatchingFramebuffers + 1;
}

std::string FramebufferCache::_getHash(int width, int height, bool onlyTexture, const TextureAttributes textureAttributes) const {
    if (onlyTexture)
    {
        return str_format("%.1dx%.1d-%d:%d:%d:%d:%d:%d:%d-NOFB", width, height, textureAttributes.minFilter, textureAttributes.magFilter, textureAttributes.wrapS, textureAttributes.wrapT, textureAttributes.internalFormat, textureAttributes.format, textureAttributes.type);
    }
    else
    {
        return str_format("%.1dx%.1d-%d:%d:%d:%d:%d:%d:%d", width, height, textureAttributes.minFilter, textureAttributes.magFilter, textureAttributes.wrapS, textureAttributes.wrapT, textureAttributes.internalFormat, textureAttributes.format, textureAttributes.type);
    }
}

Framebuffer* FramebufferCache::_getFramebufferByHash(const std::string& hash) {
    return _framebuffers[hash];
}

void FramebufferCache::purge() {
    for(const auto kvp : _framebuffers)
    {
        delete kvp.second;
    }
    _framebuffers.clear();
    _framebufferTypeCounts.clear();
}

NS_GI_END
