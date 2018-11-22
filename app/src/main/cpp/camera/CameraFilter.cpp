#include <android/native_window.h>
#include <string>
#include "CameraFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES2/gl2ext.h>
#include <src/main/cpp/filter/advanced/MagicCoolFilter.h>
#include <src/main/cpp/filter/advanced/MagicCalmFilter.h>
#include <src/main/cpp/filter/gpuimage/CameraInputFilter.h>


#define LOG_TAG "CameraFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

const static GLfloat VERTICES[]= {
        -1.0f,1.0f,
        1.0f,1.0f,
        -1.0f,-1.0f,
        1.0f,-1.0f
};

const static GLfloat TEX_COORDS[]={
        0.0f,1.0f,
        1.0f,1.0f,
        0.0f,0.0f,
        1.0f,0.0f
};

const static GLuint ATTRIB_POSITION = 0;
const static GLuint ATTRIB_TEXCOORD = 1;
const static GLuint VERTEX_NUM = 4;
const static GLuint VERTEX_POS_SIZE = 2;
const static GLuint TEX_COORD_POS_SZIE = 2;

CameraFilter::CameraFilter(ANativeWindow *window): mWindow(window),mEGLCore(new EGLCore()),
                                                   mAssetManager(nullptr),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;
}

CameraFilter::CameraFilter(ANativeWindow *window,AAssetManager* assetManager): mWindow(window),mEGLCore(new EGLCore()),
                                                   mAssetManager(assetManager),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0),filter(nullptr),cameraInputFilter(nullptr){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;
    setFilter(assetManager);
}

CameraFilter::~CameraFilter() {
    if (mWindow){
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }

    if (mEGLCore){
        delete mEGLCore;
        mEGLCore = nullptr;
    }
    mAssetManager = nullptr;

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
}

void CameraFilter::setFilter(AAssetManager* assetManager) {
    if (cameraInputFilter == nullptr){
        cameraInputFilter = new CameraInputFilter(assetManager);
    }
    if(filter != nullptr){
        filter->destroy();
    }
    filter = new MagicCalmFilter(assetManager);
    ALOGD("set filter success");
}

void CameraFilter::setFilter(std::string filterName) {

}

int CameraFilter::create() {
    glDisable(GL_DITHER);
    glClearColor(0,0,0,0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    if (!mEGLCore->buildContext(mWindow)){
        return -1;
    }

    //相机初始化
    if (cameraInputFilter!= nullptr){
        cameraInputFilter->init();
    }

    //滤镜初始化
    if (filter!= nullptr)
        filter->init();
    mTextureId = getExternalOESTextureID();
    ALOGD("get textureId success");

    return mTextureId;
}

void CameraFilter::change(int width, int height) {
    glViewport(0,0,width,height);
    mWidth = width;
    mHeight = height;
    if (cameraInputFilter!= nullptr){
        cameraInputFilter->onInputSizeChanged(width, height);
        if (filter != nullptr){
            cameraInputFilter->initCameraFrameBuffer(width,height);
        } else{
            cameraInputFilter->destroyCameraFrameBuffers();
        }
    }
}


void CameraFilter::draw(GLfloat *matrix) {
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (filter != nullptr && cameraInputFilter != nullptr){
//        cameraInputFilter->onDrawFrame(mTextureId,matrix,VERTICES,TEX_COORDS);
        GLuint id = cameraInputFilter->onDrawToTexture(mTextureId,matrix);
        filter->onDrawFrame(id,matrix);
        //缓冲区交换
        glFlush();
        mEGLCore->swapBuffer();
    }
}

void CameraFilter::stop() {
    //删除纹理
    glDeleteTextures(1,&mTextureId);
    //停止程序
    glDeleteProgram(mProgram);
    //清空资源
    mEGLCore->release();

}

void CameraFilter::setAssetManager(AAssetManager *assetManager) {
    mAssetManager = assetManager;
    setFilter(mAssetManager);
}