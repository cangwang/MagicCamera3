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

#ifndef Context_hpp
#define Context_hpp

#include "macros.h"
#include "FramebufferCache.hpp"
#include <mutex>
#include <pthread.h>
#include "GLProgram.hpp"
#include "../effect/Filter.hpp"

#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#endif

NS_GI_BEGIN

class Context {
public:
    Context();
    ~Context();

    static void init();
    static void destroy();

    static Context* getInstance();

    FramebufferCache* getFramebufferCache() const;
    void setActiveShaderProgram(GLProgram* shaderProgram);
    void purge();
    
#if PLATFORM == PLATFORM_IOS
    void runSync(std::function<void(void)> func);
    void runAsync(std::function<void(void)> func);
    void useAsCurrent(void);
    dispatch_queue_t getContextQueue() const { return _contextQueue; };
    EAGLContext* getEglContext() const { return _eglContext; };
    void presentBufferForDisplay();
#endif

    // used for capturing a processed frame data
    bool isCapturingFrame;
    Filter* captureUpToFilter;
    unsigned char* capturedFrameData;
    int captureWidth;
    int captureHeight;

private:
    static Context* _instance;
    static std::mutex _mutex;
    FramebufferCache* _framebufferCache;
    GLProgram* _curShaderProgram;
    
#if PLATFORM == PLATFORM_IOS
    dispatch_queue_t _contextQueue;
    EAGLContext* _eglContext;
#endif	
};



NS_GI_END

#endif /* Context_hpp */
