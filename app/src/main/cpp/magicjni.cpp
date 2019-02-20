#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <mutex>
#include <thread>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/filter/MagicFilterFactory.h>
#include <src/main/cpp/image/ImageFilter.h>
#include "src/main/cpp/camera/CameraEngine.h"
#include "src/main/cpp/camera/CameraFilter.h"

#define LOG_TAG "magicjni"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
extern "C" {

std::mutex gMutex;
CameraEngine *glCamera = nullptr;
CameraFilter *glCameraFilter = nullptr;
ImageFilter *glImageFilter = nullptr;
AAssetManager *aAssetManager = nullptr;

JNIEXPORT jint JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseInit(JNIEnv *env, jobject obj,
                                                        jobject surface,jint width,jint height,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCamera){
        glCamera->stop();
        delete glCamera;
    }
    //创建window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //获取文件管理
    AAssetManager *manager = AAssetManager_fromJava(env,assetManager);
    glCamera = new CameraEngine(window);
    glCamera->setAssetManager(manager);
    glCamera->resize(width,height);
    return glCamera->create();
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseCreate(JNIEnv *env, jobject obj) {
    //关闭颜色抖动 若机器的分辨率已经相当高，激活抖动操作根本就没有任何意义
    glDisable(GL_DITHER);
    //清颜色
    glClearColor(0.0,0.0,0.0,0.0);
    //开启剔除操作效果 在多边形中使用
    glEnable(GL_CULL_FACE);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseChange(JNIEnv *env, jobject obj,jint width,jint height) {
    //视口变换，可视区域
    if(glCamera)
        glCamera->resize(width,height);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseDraw(JNIEnv *env, jobject obj,jfloatArray matrix_) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);

    std::unique_lock<std::mutex> lock(gMutex);
    if (!glCamera){
        ALOGE("draw error, glCamera is null");
        return;
    }
    glCamera->draw(matrix);
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseRelease(JNIEnv *env, jobject obj) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glCamera){
        glCamera->stop();
        delete glCamera;
        glCamera = nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicAdjustSize(JNIEnv *env, jobject obj,jint orientation,jboolean isFront,jboolean flipVertical) {

}

//相机滤镜surfaceView初始化的时候创建
JNIEXPORT jint JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterCreate(JNIEnv *env, jobject obj,
                                                        jobject surface,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCameraFilter){ //停止摄像头采集并销毁
        glCameraFilter->stop();
        delete glCameraFilter;
    }

    //初始化native window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //初始化app内获取数据管理
    aAssetManager= AAssetManager_fromJava(env,assetManager);
    //初始化相机采集
    glCameraFilter = new CameraFilter(window,aAssetManager);
    //创建
    return glCameraFilter->create();
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterSet(JNIEnv *env, jobject obj,
                                                            jobject surface,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (!glCameraFilter){
        ALOGE("filter set error, glCameraFilter is null");
        return;
    }
    AAssetManager *manager = AAssetManager_fromJava(env,assetManager);
    glCameraFilter->setFilter(manager);
}

//窗口大小设置，SurfaceView初始化后会触发一次
JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterChange(JNIEnv *env, jobject obj,jint width,jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glCameraFilter){
        ALOGE("change error, glCameraFilter is null");
        return;
    }
    //更改窗口大小
    glCameraFilter->change(width,height);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterDraw(JNIEnv *env, jobject obj,jfloatArray matrix_,jstring address) {
    //获取摄像头矩阵
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);
    //加锁
    std::unique_lock<std::mutex> lock(gMutex);
    //如果为空，就判断错误，中断
    if (!glCameraFilter){
        ALOGE("draw error, glCameraFilter is null");
        return;
    }
    //摄像头采集画图
    glCameraFilter->draw(matrix);
    //释放矩阵数据
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterRelease(JNIEnv *env, jobject obj) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glCameraFilter){
        glCameraFilter->stop();
        delete glCameraFilter;
        glCameraFilter = nullptr;
    }
}

//图片滤镜surfaceView初始化的时候创建
JNIEXPORT jint JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicImageFilterCreate(JNIEnv *env, jobject obj,
                                                            jobject surface,jobject assetManager,jstring imgPath,jint degree) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glImageFilter){ //停止摄像头采集并销毁
        glImageFilter->stop();
        delete glImageFilter;
    }

    //初始化native window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //初始化app内获取数据管理
    aAssetManager= AAssetManager_fromJava(env,assetManager);
    //初始化图片
    const char* addressStr = env->GetStringUTFChars(imgPath,0);
    std::string nativeAddress = addressStr;

    glImageFilter = new ImageFilter(window,aAssetManager,nativeAddress,degree);
    env->ReleaseStringUTFChars(imgPath, addressStr);
    //创建
    return glImageFilter->create();
}

//窗口大小设置，SurfaceView初始化后会触发一次
JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicImageFilterChange(JNIEnv *env, jobject obj,jint width,jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glImageFilter){
        ALOGE("change error, glImageFilter is null");
        return;
    }
    //更改窗口大小
    glImageFilter->change(width,height);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicImageFilterDraw(JNIEnv *env, jobject obj,jfloatArray matrix_,jstring address) {
    //获取摄像头矩阵
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);
    //加锁
    std::unique_lock<std::mutex> lock(gMutex);
    //如果为空，就判断错误，中断
    if (!glImageFilter){
        ALOGE("draw error, glImageFilter is null");
        return;
    }
    //图片和滤镜绘制
    glImageFilter->draw(matrix);
    //释放矩阵数据
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicImageFilterRelease(JNIEnv *env, jobject obj) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glImageFilter){
        glImageFilter->stop();
        delete glImageFilter;
        glImageFilter = nullptr;
    }
}


JNIEXPORT jintArray JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_getFilterTypes(JNIEnv *env, jobject obj) {
    int len =0;
    jint* types = getFilterTypes(len);
    jintArray jin_arr=(env)->NewIntArray(len);
    (env)->SetIntArrayRegion(jin_arr,0,len,types);
    return jin_arr;
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_setFilterType(JNIEnv *env, jobject obj,jint type) {
   if(glCameraFilter!= nullptr)
       glCameraFilter->setFilter(type);
   else ALOGE("filter set error, glCameraFilter is null");
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_setImageFilterType(JNIEnv *env, jobject obj,jint type) {
    if(glImageFilter!= nullptr)
        glImageFilter->setFilter(type);
    else ALOGE("filter set error, glImageFilter is null");
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_setImageBeautyLevel(JNIEnv *env, jobject obj,jint level) {
    if(glImageFilter!= nullptr)
        glImageFilter->setBeautyLevel(level);
    else ALOGE("beauty set error, glCameraFilter is null");
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_setBeautyLevel(JNIEnv *env, jobject obj,jint level) {
    if(glCameraFilter!= nullptr)
        glCameraFilter->setBeautyLevel(level);
    else ALOGE("beauty set error, glCameraFilter is null");
}

JNIEXPORT jboolean JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_savePhoto(JNIEnv *env, jobject obj,jstring address) {
    if (glCameraFilter != nullptr) {
        const char* addressStr = env->GetStringUTFChars(address,0);
        std::string nativeAddress = addressStr;
        bool result = false;
        if(glCameraFilter!= nullptr)
            result = glCameraFilter->savePhoto(nativeAddress);
        env->ReleaseStringUTFChars(address, addressStr);
        return static_cast<jboolean>(result);
    } else{
        ALOGE("save photo error, glCameraFilter is null");
        return static_cast<jboolean>(false);
    }
}

JNIEXPORT jboolean JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_saveImage(JNIEnv *env, jobject obj,jstring address) {
    if (glImageFilter != nullptr) {
        const char* addressStr = env->GetStringUTFChars(address,0);

        std::string nativeAddress = addressStr;
        bool result = false;
        if(glImageFilter!= nullptr)
            result = glImageFilter->saveImage(nativeAddress);
        env->ReleaseStringUTFChars(address, addressStr);
        return static_cast<jboolean>(result);
    }else{
        ALOGE("save photo error, glImageFilter is null");
        return static_cast<jboolean>(false);
    }
}

}
