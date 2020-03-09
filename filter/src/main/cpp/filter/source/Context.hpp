#ifndef Context_hpp
#define Context_hpp

#include "macros.hpp"
#include "FramebufferCache.hpp"
#include <mutex>
#include <pthread.h>
#include "GLProgram.hpp"
#include "Filter.hpp"

#if PLATFORM == PLATFORM_IOS
#import <OpengGLES/EGAL.h>
#import <OpengGLES/ES3/gl.h>
#endif

/**
 * cangwang 2020.3.5
 */
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
    void runSync(std::function<void(void) func);
    void runAsync(std::function<void(void)> func);
    void useAsCurrent(void);
    dispatch_queue_t getContextQueue() const { return _contextQueue;};
    EAGLContext* getEglContext() const { return _eglContext; };
    vod presentBufferForDisplay();
#endif

    //用于采集一帧的数据
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
#endif