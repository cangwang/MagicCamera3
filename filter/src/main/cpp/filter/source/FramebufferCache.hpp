#ifndef FramebufferCache_hpp
#define FramebufferCache_hpp

#include "macros.hpp"
#include "Framebuffer.hpp"
#include <string>
#include <map>

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN

class FramebufferCache {
public:
    FramebufferCache();
    ~FramebufferCache();
    Framebuffer* fetchFramebuffer(int width, int height, bool onlyTexture = false,
            const TextureAttributes textureAttributes = Framebuffer:: defaultTextureAttributes );
    void returnFramebuffer(Framebuffer* framebuffer);
    void purge();

private:
    std::string _getHash(int width, int height, bool onlyTexture,
            const TextureAttributes textureAttributes) const ;
    Framebuffer* _getFramebufferByHash(const std::string& hash);

    std::map<std::string, Framebuffer*> _framebuffers;
    std::map<std::string, int> _framebufferTypeCounts;
};

NS_GI_END

#endif