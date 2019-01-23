#include <android/native_window.h>
#include <string>
#include "ImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES2/gl2ext.h>
#include <src/main/cpp/filter/gpuimage/GpuImageFilter.h>
#include <src/main/cpp/filter/advanced/MagicNoneFilter.h>
#include <src/main/cpp/filter/MagicFilterFactory.h>


#define LOG_TAG "ImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

/**
 * cangwang 2018.12.1
 */

ImageFilter::ImageFilter(){
}

ImageFilter::ImageFilter(ANativeWindow *window,AAssetManager* assetManager,std::string path,int degree): mWindow(window),mEGLCore(new EGLCore()),
                                                   mAssetManager(assetManager),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0),filter(nullptr),imageInput(nullptr),beautyFilter(nullptr),imgPath(path),degree(degree){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;

    if (imageInput == nullptr){
        imageInput = new ImageInput(assetManager,path);
    }
    setFilter(assetManager);
}

ImageFilter::~ImageFilter() {
    if (filter!= nullptr){
        filter->destroy();
        delete filter;
        filter = nullptr;
    }

    if (imageInput!= nullptr){
        imageInput->destroyFrameBuffers();
        imageInput->destroy();
        delete imageInput;
        imageInput = nullptr;
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
}

void ImageFilter::setFilter(AAssetManager* assetManager) {
    if(filter != nullptr){
        filter->destroy();
    }
    filter = new MagicNoneFilter(assetManager);
    //调整滤镜中的图片的方向问题
    filter->setOrientation(degree);
    ALOGD("set filter success");
}

void ImageFilter::setFilter(int type) {
    GPUImageFilter* filter = initFilters(type,mAssetManager);
    setFilter(filter);
}

int ImageFilter::create() {
    glDisable(GL_DITHER);
    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (!mEGLCore->buildContext(mWindow)){
        return -1;
    }

    //相机初始化
    if (imageInput!= nullptr){
        imageInput->init();
    }

    //滤镜初始化
    if (filter!= nullptr)
        filter->init();
    mTextureId = getExternalOESTextureID();
    ALOGD("get textureId success");

    return mTextureId;
}

void ImageFilter::change(int width, int height) {
    //设置视口
    glViewport(0,0,width,height);
    mWidth = width;
    mHeight = height;
    if (imageInput!= nullptr){
        if (imageInput!= nullptr){
            //触发输入大小更新
            imageInput->onInputSizeChanged(width, height);
            //初始化帧缓冲
//            imageInput->initFrameBuffer(width,height);
        }
        if (filter != nullptr){
            //初始化滤镜的大小
            filter->onInputSizeChanged(width,height);
        } else{
            imageInput->destroyFrameBuffers();
        }
    }
}


void ImageFilter::draw(GLfloat *matrix) {
    //清屏
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (imageInput != nullptr &&filter!= nullptr){
        //通过滤镜filter绘制
//        GLuint id = imageInput->onDrawToTexture(imageInput->imgTexture,matrix);
//        filter->onDrawFrame(id,matrix);
        filter->onDrawFrame(imageInput->imgTexture,matrix);
        //缓冲区交换
        glFlush();
        mEGLCore->swapBuffer();
    }
}

void ImageFilter::stop() {
    //删除纹理
    glDeleteTextures(1,&mTextureId);
}

void ImageFilter::setFilter(GPUImageFilter* gpuImageFilter) {
    if(filter != nullptr){
        filter->destroy();
        filter= nullptr;
    }
    filter = gpuImageFilter;
    ALOGD("set filter success");
    if (filter!= nullptr)
        filter->init();
    filter->onInputSizeChanged(imageInput->mInputWidth,imageInput->mInputHeight);
}

void ImageFilter::setBeautyLevel(int level) {
    if (imageInput != nullptr){
        imageInput->setBeautyLevel(level);
    }
}

bool ImageFilter::savePhoto(std::string saveFileAddress){
    if(filter != nullptr){
        return filter->savePhoto(saveFileAddress);
    }
    return false;
}