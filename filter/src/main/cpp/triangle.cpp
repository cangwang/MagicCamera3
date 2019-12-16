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
    shader = glCreateShader(type);

    if(shader ==0){
        return 0;
    }

    glShaderSource(shader,1,&shaderSrc,NULL);
    glCompileShader(shader);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);

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
    return shader;
}


JNIEXPORT void JNICALL
Java_com_cangwang_magic_util_RenderJNI_glesInit(JNIEnv *env, jobject obj) {

    char vShaderStr[] =
            "#version 300 es                        \n"
                    "layout(location = 0)in vec4 vPosition; \n"
                    "void main()                            \n"
                    "{                                      \n"
                    " gl_Position = vPosition;              \n"
                    "}                                      \n";

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

    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    programObject = glCreateProgram();
    if (programObject == 0) {
        return;
    }

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glLinkProgram(programObject);

    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char *) * infoLen);
            glGetShaderInfoLog(programObject, infoLen, NULL, infoLog);
            LOGE("Error compiling shader:[%s]", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return;
    }

    g_programObject = programObject;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

JNIEXPORT void JNICALL
Java_com_cangwang_filter_util_RenderJNI_glesRender(JNIEnv *env, jobject obj) {
    GLfloat vVertices[] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    glViewport(0, 0, g_width, g_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(g_programObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


JNIEXPORT void JNICALL
Java_com_cangwang_filter_util_RenderJNI_glesResize(JNIEnv *env, jobject obj, jint width,
                                                  jint height) {
    g_width = width;
    g_height = height;
}

JNIEXPORT void JNICALL
Java_com_cangwang_filter_util_RenderJNI_readShaderFile(JNIEnv *env, jobject obj,
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
