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

#include "Context.hpp"
#include "util.h"

#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/glext.h>
#endif

NS_GI_BEGIN

#if PLATFORM == PLATFORM_IOS
#define GL_CONTEXT_QUEUE    "com.cangwang.GPUImage-x.openglESContextQueue"
#endif

Context* Context::_instance = 0;
std::mutex Context::_mutex;

Context::Context()
:_curShaderProgram(0)
,isCapturingFrame(false)
,captureUpToFilter(0)
,capturedFrameData(0)
{
    _framebufferCache = new FramebufferCache();
    
#if PLATFORM == PLATFORM_IOS
    _contextQueue = dispatch_queue_create(GL_CONTEXT_QUEUE, DISPATCH_QUEUE_SERIAL);
    _eglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    [EAGLContext setCurrentContext:_eglContext];
#endif	
}

Context::~Context() {
    delete _framebufferCache;
}

Context* Context::getInstance() {
    if (!_instance)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_instance) {
            _instance = new (std::nothrow) Context;
        }
    }
    return _instance;
};

void Context::init() {
    destroy();
    getInstance();
}

void Context::destroy() {
    if (_instance) {
        delete _instance;
        _instance = 0;
    }
}

FramebufferCache* Context::getFramebufferCache() const {
    return _framebufferCache;
}

void Context::setActiveShaderProgram(GLProgram* shaderProgram) {
    if (_curShaderProgram != shaderProgram)
    {
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
