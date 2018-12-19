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
#include "src/main/cpp/camera/CameraEngine.h"
#include "src/main/cpp/camera/CameraFilter.h"

#define LOG_TAG "magicjni"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
extern "C" {

std::mutex gMutex;
CameraEngine *glCamera = nullptr;
CameraFilter *glCameraFilter = nullptr;
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

JNIEXPORT jint JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterCreate(JNIEnv *env, jobject obj,
                                                        jobject surface,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCameraFilter){
        glCameraFilter->stop();
        delete glCameraFilter;
    }

    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    aAssetManager= AAssetManager_fromJava(env,assetManager);
    glCameraFilter = new CameraFilter(window,aAssetManager);
//    glCameraFilter->setAssetManager(manager);
//    glCameraFilter->resize(width,height);

    return glCameraFilter->create();
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterSet(JNIEnv *env, jobject obj,
                                                            jobject surface,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (!glCameraFilter){
        ALOGE("draw error, glCamera is null");
        return;
    }
    AAssetManager *manager = AAssetManager_fromJava(env,assetManager);
    glCameraFilter->setFilter(manager);
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterChange(JNIEnv *env, jobject obj,jint width,jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glCameraFilter){
        ALOGE("draw error, glCamera is null");
        return;
    }
    glCameraFilter->change(width,height);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicFilterDraw(JNIEnv *env, jobject obj,jfloatArray matrix_) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);

    std::unique_lock<std::mutex> lock(gMutex);
    if (!glCameraFilter){
        ALOGE("draw error, glCamera is null");
        return;
    }
    glCameraFilter->draw(matrix);
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
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_setBeautyLevel(JNIEnv *env, jobject obj,jint level) {
    if(glCameraFilter!= nullptr)
        glCameraFilter->setBeautyLevel(level);
}

}
