#include <android/native_window.h>
#include <string>
#include "VideoFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES2/gl2ext.h>
#include <src/main/cpp/filter/advanced/MagicCoolFilter.h>
#include <src/main/cpp/filter/advanced/MagicAmaroFilter.h>
#include <src/main/cpp/filter/advanced/MagicCalmFilter.h>
#include <src/main/cpp/filter/advanced/MagicNoneFilter.h>
#include <src/main/cpp/filter/gpuimage/CameraInputFilterV2.h>
#include <src/main/cpp/filter/MagicFilterFactory.h>


#define LOG_TAG "VideoFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

/**
 * 相机滤镜管理
 * cangwang 2018.12.1
 */
VideoFilter::VideoFilter(ANativeWindow *window): mWindow(window),mEGLCore(new EGLCore()),
                                                   mAssetManager(nullptr),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;
}

VideoFilter::VideoFilter(ANativeWindow *window,AAssetManager* assetManager): mWindow(window),mEGLCore(new EGLCore()),mVideoWindow(nullptr),
                                                   mAssetManager(assetManager),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0),filter(nullptr),cameraInputFilter(nullptr),
                                                   pool(new std::MagicThreadPool()){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;

    if (cameraInputFilter == nullptr){
        cameraInputFilter = new CameraInputFilter(assetManager);
    }

    setFilter(assetManager);
}

VideoFilter::~VideoFilter() {
    if (filter!= nullptr){
        filter->destroy();
        delete filter;
        filter = nullptr;
    }

    if (cameraInputFilter!= nullptr){
        cameraInputFilter->destroyCameraFrameBuffers();
        cameraInputFilter->destroy();
        delete cameraInputFilter;
        cameraInputFilter = nullptr;
    }

    if (mEGLCore){
        //清空资源
        mEGLCore->release();
        delete mEGLCore;
        mEGLCore = nullptr;
    }
    if (mWindow){
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }

    mAssetManager = nullptr;
    //释放线程池
    free(pool);
}

void VideoFilter::setFilter(AAssetManager* assetManager) {
    if(filter != nullptr){
        filter->destroy();
    }
    filter = new MagicNoneFilter(assetManager);
    filter->setPool(pool);
    ALOGD("set filter success");
}

void VideoFilter::setFilter(int type) {
    GPUImageFilter* filter = initFilters(type,mAssetManager);
    setFilter(filter);
}

int VideoFilter::create(int textureId,EGLContext eglContext) {
    glDisable(GL_DITHER);
    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (!mEGLCore->buildVideoContext(mWindow,eglContext)){
        return -1;
    }

    //相机初始化
    if (cameraInputFilter!= nullptr){
        cameraInputFilter->init();
    }

    //滤镜初始化
    if (filter!= nullptr)
        filter->init();
    mTextureId = static_cast<GLuint>(textureId);
    ALOGD("get textureId success");

    return mTextureId;
}

void VideoFilter::change(int width, int height) {
    //设置视口
    glViewport(0,0,width,height);
    mWidth = width;
    mHeight = height;

    if (cameraInputFilter!= nullptr){
        if (cameraInputFilter!= nullptr){
            //触发输入大小更新
            cameraInputFilter->onInputSizeChanged(width, height);
            //初始化帧缓冲
            cameraInputFilter->initCameraFrameBuffer(width,height);
        }
        if (filter != nullptr){
            //初始化滤镜的大小
            filter->onInputSizeChanged(width,height);
        } else{
            cameraInputFilter->destroyCameraFrameBuffers();
        }
    }
}


void VideoFilter::draw(GLfloat *matrix,long time) {
    //清屏
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (cameraInputFilter != nullptr){
        //获取帧缓冲id
        GLuint id = cameraInputFilter->onDrawToTexture(mTextureId,matrix);
        if (filter != nullptr)
            //通过滤镜filter绘制
            filter->onDrawFrame(id,matrix);
        //缓冲区交换
        glFlush();
        if(mEGLCore!= nullptr) {
            //设置时间戳的方法暂时有问题，会导致录制一段时间后无法录制
//            mEGLCore->setPresentationTime(time);
            mEGLCore->swapBuffer();
            ALOGD("set video draw");
        }
    }
}

void VideoFilter::stop() {
    //删除纹理
    glDeleteTextures(1,&mTextureId);
}

void VideoFilter::setFilter(GPUImageFilter* gpuImageFilter) {
    if(filter != nullptr){
        filter->destroy();
        filter= nullptr;
    }
    filter = gpuImageFilter;
    filter->setPool(pool);
    ALOGD("set filter success");
    if (filter!= nullptr)
        filter->init();
    filter->onInputSizeChanged(cameraInputFilter->mScreenWidth,cameraInputFilter->mScreenHeight);
}

void VideoFilter::setBeautyLevel(int level) {
    if (cameraInputFilter != nullptr){
        cameraInputFilter->setBeautyLevel(level);
    }
}

bool VideoFilter::savePhoto(std::string saveFileAddress){
    if(filter != nullptr){
        return filter->savePhoto(saveFileAddress);
    }
    return false;
}

