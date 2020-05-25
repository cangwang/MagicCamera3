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

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "macros.h"
#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#elif PLATFORM == PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include <vector>
#include "Ref.hpp"

NS_GI_BEGIN

typedef struct {
    GLenum minFilter;
    GLenum magFilter;
    GLenum wrapS;
    GLenum wrapT;
    GLenum internalFormat;
    GLenum format;
    GLenum type;
} TextureAttributes;


class Framebuffer : public Ref{
public:
    Framebuffer(int width, int height, bool onlyGenerateTexture = false, const TextureAttributes textureAttributes = defaultTextureAttribures);
    ~Framebuffer();
    
    virtual void release(bool returnToCache = true);
    
    GLuint getTexture() const {
        return _texture;
    }

    GLuint getFramebuffer() const {
        return _framebuffer;
    }
    
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    const TextureAttributes& getTextureAttributes() const { return _textureAttributes; };
    bool hasFramebuffer() { return _hasFB; };
    
    void active();
    void inactive();

    static TextureAttributes defaultTextureAttribures;
    
private:
    int _width, _height;
    TextureAttributes _textureAttributes;
    bool _hasFB;
    GLuint _texture;
    GLuint _framebuffer;
    
    void _generateTexture();
    void _generateFramebuffer();

    static std::vector<Framebuffer*> _framebuffers;
};


NS_GI_END

#endif /* Framebuffer_hpp */
