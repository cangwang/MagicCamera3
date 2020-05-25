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

#ifndef FramebufferCache_hpp
#define FramebufferCache_hpp

#include "macros.h"
#include "Framebuffer.hpp"
#include <string>
#include <map>

NS_GI_BEGIN

class FramebufferCache {
public:
    FramebufferCache();
    ~FramebufferCache();
    Framebuffer* fetchFramebuffer(int width, int height, bool onlyTexture = false, const TextureAttributes textureAttributes = Framebuffer::defaultTextureAttribures );
    void returnFramebuffer(Framebuffer* framebuffer);
    void purge();
    
    
private:
    std::string _getHash(int width, int height, bool onlyTexture, const TextureAttributes textureAttributes) const;
    Framebuffer* _getFramebufferByHash(const std::string& hash);
    
    std::map<std::string, Framebuffer*> _framebuffers;
    std::map<std::string, int> _framebufferTypeCounts;
    
};

NS_GI_END

#endif /* FramebufferCache_hpp */
