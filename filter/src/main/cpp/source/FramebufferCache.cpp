#include "FramebufferCache.hpp"
#include "util.h"
/**
 * cangwang 2020.3.5
 */

NS_GI_BEGIN

FramebufferCache::FramebufferCache() {

}

FramebufferCache::~FramebufferCache() {
    purge();
}

//刷新帧缓冲
Framebuffer* FramebufferCache::fetchFramebuffer(int width, int height, bool onlyTexture,
                                                const GPUImage::TextureAttributes textureAttributes) {
    Framebuffer* framebufferFromCache = nullptr;
    //确定key值
    std::string lookupHash = _getHash(width, height, onlyTexture, textureAttributes);
    int numberOfMatchingFramebuffers = 0;
    //判断帧缓冲对应的id
    if (_framebufferTypeCounts.find(lookupHash) != _framebufferTypeCounts.end()) {
        numberOfMatchingFramebuffers = _framebufferTypeCounts[lookupHash];
    }

    if (numberOfMatchingFramebuffers < 1) {
        //创建帧缓冲
        framebufferFromCache = new Framebuffer(width ,height, onlyTexture, textureAttributes);
    } else {
        //TODO 无法解析
        int curFramebufferId = numberOfMatchingFramebuffers - 1;
        while (!framebufferFromCache && curFramebufferId >= 0) {
            std::string framebufferHash = str_format("%s-%ld", lookupHash.c_str(), curFramebufferId);
            if (_framebuffers.find(framebufferHash) != _framebuffers.end()) {
                framebufferFromCache = _framebuffers[framebufferHash];
                _framebuffers.erase(framebufferHash);
            } else {
                framebufferFromCache = nullptr;
            }
            curFramebufferId--;
        }
        curFramebufferId++;
        _framebufferTypeCounts[lookupHash] = curFramebufferId;

        if (!framebufferFromCache) {
            framebufferFromCache = new Framebuffer(width, height, onlyTexture, textureAttributes);
        }
    }

    //重置引用
    framebufferFromCache->resetReferenceCount();
    return framebufferFromCache;
}

void FramebufferCache::returnFramebuffer(GPUImage::Framebuffer *framebuffer) {
    if (framebuffer == 0) return;
    int width = framebuffer->getWidth();
    int height = framebuffer->getHeight();
    const TextureAttributes& textureAttributes = framebuffer->getTextureAttributes();
    std::string lookupHash = _getHash(width, height, !framebuffer->hasFramebuffer(),
            textureAttributes);
    int numberOfMatchingFramebuffers = 0;
    if (_framebufferTypeCounts.find(lookupHash) != _framebufferTypeCounts.end()){
        numberOfMatchingFramebuffers = _framebufferTypeCounts[lookupHash];
    }
    std::string framebufferHash = str_format("%s-%ld", lookupHash.c_str(), numberOfMatchingFramebuffers);
    _framebuffers[framebufferHash] = framebuffer;
    _framebufferTypeCounts[lookupHash] = numberOfMatchingFramebuffers + 1;
}

//获取framebufferTypeCounts的key值
std::string FramebufferCache::_getHash(int width, int height, bool onlyTexture,
                                       const GPUImage::TextureAttributes textureAttributes) const {
    if (onlyTexture)
    {
        return str_format("%.1dx%.1d-%d:%d:%d:%d:%d:%d:%d-NOFB", width, height, textureAttributes.minFilter, textureAttributes.magFilter,
                textureAttributes.wrapS, textureAttributes.wrapT, textureAttributes.internalFormat, textureAttributes.format, textureAttributes.type);
    }
    else
    {
        return str_format("%.1dx%.1d-%d:%d:%d:%d:%d:%d:%d", width, height, textureAttributes.minFilter, textureAttributes.magFilter,
                textureAttributes.wrapS, textureAttributes.wrapT, textureAttributes.internalFormat, textureAttributes.format, textureAttributes.type);
    }
}

Framebuffer* FramebufferCache::_getFramebufferByHash(const std::string &hash) {
    return _framebuffers[hash];
}

void FramebufferCache::purge() {
    for(const auto kvp: _framebuffers) {
        delete kvp.second;
    }
    _framebuffers.clear();
    _framebufferTypeCounts.clear();
}


NS_GI_END