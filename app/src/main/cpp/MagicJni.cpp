#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>

extern "C" {


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_OpenGLJniLib_magicBaseInit(JNIEnv *env, jobject obj) {

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
    glViewport(0,0,width,height);

}

}

