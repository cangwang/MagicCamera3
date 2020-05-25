#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>

#define LOG_TAG "ndk-build"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {
GLint g_programObject;
jint g_width;
jint g_height;

AAssetManager * g_aAssetManager = NULL;

GLuint LoadShader(GLenum type,const char *shaderSrc){
    GLuint shader;
    GLint compiled;
    //通过shader类型来获取
    shader = glCreateShader(type);

    if(shader ==0){
        return 0;
    }

    //加载shader资源
    glShaderSource(shader,1,&shaderSrc,NULL);
    //编译shader资源
    glCompileShader(shader);
    //获取shader信息
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);

    //编译不成功，打印日志
    if(!compiled){
        GLint infoLen = 0;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);

        if(infoLen >1){
            char *infoLog= (char*)malloc(sizeof(char*) *infoLen);
            glGetShaderInfoLog(shader,infoLen,NULL,infoLog);
            LOGE("Error compiling shader:[%s]",infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    //返回编译成功的shader
    return shader;
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_RenderJNI_glesInit(JNIEnv *env, jobject obj) {

    //顶点着色器内容
    char vShaderStr[] =
            "#version 300 es                        \n"
                    "layout(location = 0)in vec4 vPosition; \n"
                    "void main()                            \n"
                    "{                                      \n"
                    " gl_Position = vPosition;              \n"
                    "}                                      \n";

    //片元着色器内容
    char fShaderStr[] =
            "#version 300 es                        \n"
                    "precision mediump float;               \n"
                    "out vec4 fragColor;                    \n"
                    "void main()                            \n"
                    "{                                      \n"
                    "   fragColor = vec4(1.0,0.0,0.0,1.0);  \n"
                    "}                                      \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    //加载顶点着色器
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    //加载片元着色器
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    //创建程序
    programObject = glCreateProgram();
    if (programObject == 0) {
        return;
    }

    //绑定顶点着色器
    glAttachShader(programObject, vertexShader);
    //绑定片元着色器
    glAttachShader(programObject, fragmentShader);

    //关联到程序
    glLinkProgram(programObject);

    //获取程序状态
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    //如果没有链接成功
    if (!linked) {
        GLint infoLen = 0;
        //获取错误日志
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        //错误日志有内容
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char *) * infoLen);
            //读取对应长度内容
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            //打印
            LOGE("Error compiling shader:[%s]", infoLog);
            //释放log
            free(infoLog);
        }
        //关闭程序
        glDeleteProgram(programObject);
        return;
    }
    //转为全局地址
    g_programObject = programObject;

    //清屏幕
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_RenderJNI_glesRender(JNIEnv *env, jobject obj) {
    //顶点
    GLfloat vVertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    //清屏
    glViewport(0, 0, g_width, g_height);
    glClear(GL_COLOR_BUFFER_BIT);
    //使用程序
    glUseProgram(g_programObject);
    //绑定顶点数据到shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    //允许顶点着色器读取GPU数据
    glEnableVertexAttribArray(0);
    //画三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_RenderJNI_glesResize(JNIEnv *env, jobject obj, jint width,
                                                  jint height) {
    g_width = width;
    g_height = height;
}

JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_RenderJNI_readShaderFile(JNIEnv *env, jobject obj,
                                                      jobject assetManager) {
    if (assetManager && env) {
        g_aAssetManager = AAssetManager_fromJava(env, assetManager);

        if (NULL == g_aAssetManager) {
            LOGE("AAssetManager_fromJava() return null !");
        }
    } else {
        LOGE("assetManager is null !");
    }
}
}
