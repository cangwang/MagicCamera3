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
#include "src/main/cpp/camera/CameraEngine.h"

#define LOG_TAG "magicjni"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
extern "C" {

std::mutex gMutex;
CameraEngine *glCamera = nullptr;

JNIEXPORT jint JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseInit(JNIEnv *env, jobject obj,
                                                        jobject surface,jint width,jint height,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCamera){
        glCamera->stop();
        delete glCamera;
    }

    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
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

}

