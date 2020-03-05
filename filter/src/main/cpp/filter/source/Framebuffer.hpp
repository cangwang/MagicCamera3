#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "marcos.hpp"

#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#elif PLATFORM_ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif

#include <vector>
#include "Ref.hpp"

/**
 * cangwang 2020.3.5
 */
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

//帧缓存对象
class Framebuffer : public Ref {
public:
    Framebuffer(int width, int height, bool onlyGenerateTexture = false,
            const TextureAttributes textureAttributes = defaultTextureAttributes);
    ~Framebuffer();

    virtual void release(bool retureToCache = true);

    GLuint getTexture() const {
        return _texture;
    }

    GLuint getFramebuffer() const {
        return _framebuffer;
    }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    const TextureAttributes& getTextureAttributes() const { return _textureAttributes;}
    bool hasFramebuffer() { return _hasFB; }

    void active();
    void inactive();

    static TextureAttributes defaultTextureAttributes;
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
#endif