
#include "Context.hpp"
#include "util.h"

#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES3/glexth>
#endif

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN
#if PLATFORM == PLATFORM_IOS
#define GL_CONTEXT_QUEUE "com.jin.GPUImage-x.openglESContextQueue"
#endif

Context* Context::_instance = 0;
std::mutex Context::_mutex;

Context::Context()
:_curShaderProgram(nullptr)
,isCapturingFrame(false)
,captureUpToFilter(nullptr)
,capturedFrameData(nullptr){
    _framebufferCache = new FramebufferCache();
#if PLATFORM == PLATFORM_IOS
    _contextQueue = dispatch_queue_create(GL_CONTEXT_QUEUE,
            DISPATCH_QUEUE_SERIAL);
    _eglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderringAPIOpenGLES3];
    [EAGLContext setCurrentContext:_eglContext];
#endif
}

Context::~Context() {
    delete _framebufferCache;
}

Context* Context::getInstance() {
    if (!_instance) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_instance) {
            _instance = new (std::nothrow) Context;
        }
    }
    return _instance;
}

void Context::init() {
    destroy();
    getInstance();
}

void Context::destroy() {
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

FramebufferCache* Context::getFramebufferCache() const {
    return _framebufferCache;
}

void Context::setActiveShaderProgram(GPUImage::GLProgram *shaderProgram) {
    if (_curShaderProgram != shaderProgram) {
        _curShaderProgram = shaderProgram;
        shaderProgram->use();
    }
}

void Context::purge() {
    _framebufferCache->purge();
}

#if PLATFORM == PLATFORM_IOS
    void Context::runSync(std::function<void(void)> func) {
    useAsCurrent();
    dispatch_queue_t contextQueue = Context::getInstance()->getContextQueue();

    if (dispatch_get_current_queue() == contextQueue)
    {
        func();
    }else
    {
        dispatch_sync(contextQueue, ^{ func(); });
    }
}

void Context::runAsync(std::function<void(void)> func) {
    useAsCurrent();
    dispatch_queue_t contextQueue = Context::getInstance()->getContextQueue();

    if (dispatch_get_current_queue() == contextQueue)
    {
        func();
    }else
    {
        dispatch_async(contextQueue, ^{ func(); });
    }
}

void Context::useAsCurrent(void)
{
    if ([EAGLContext currentContext] != _eglContext)
    {
        [EAGLContext setCurrentContext:_eglContext];
    }
}

void Context::presentBufferForDisplay() {
    [_eglContext presentRenderbuffer:GL_RENDERBUFFER ];
}
#endif

NS_GI_END