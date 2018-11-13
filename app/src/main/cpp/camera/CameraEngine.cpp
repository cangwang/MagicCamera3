#include <android/native_window.h>
#include <string>
#include "CameraEngine.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES2/gl2ext.h>

#define LOG_TAG "CameraEngine"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

const static GLfloat VERTICES[]= {
        -1.0f,1.0f,
        1.0f,1.0f,
        -1.0f,1.0f,
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

CameraEngine::CameraEngine(ANativeWindow *window): mWindow(window),mEGLCore(new EGLCore()),
                                                   mAssetManager(nullptr),mTextureId(0),mTextureLoc(0),
                                                   mMatrixLoc(0){
    //清空mMatrix数组
    memset(mMatrix,0, sizeof(mMatrix));
    mMatrix[0] = 1;
    mMatrix[5] = 1;
    mMatrix[10] = 1;
    mMatrix[15] = 1;
}

CameraEngine::~CameraEngine() {
    if (mWindow){
        ANativeWindow_release(mWindow);
        mWindow = nullptr;
    }

    if (mEGLCore){
        delete mEGLCore;
        mEGLCore = nullptr;
    }

    mAssetManager = nullptr;
}

int CameraEngine::create() {
    if (!mEGLCore->buildContext(mWindow)){
        return -1;
    }
    std::string *vShader = readShaderFromAsset(mAssetManager,"");
    std::string *fShader = readShaderFromAsset(mAssetManager,"");
    //加载程序
    mProgram = loadProgram(vShader->c_str(),fShader->c_str());

    //生成纹理贴图
    glGenTextures(1,&mTextureId);
    //绑定纹理
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    mMatrixLoc = glGetUniformLocation(mProgram,"mMatrix");
    mTextureLoc = glGetUniformLocation(mProgram,"sTexture");

    glClearColor(1.0f,1.0f,1.0f,1.0f);

    delete vShader;
    delete fShader;
    return mTextureId;
}


void CameraEngine::draw(GLfloat *matrix) {
    glViewport(0,0,mWidth,mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram);
    //激活纹理
    glActiveTexture(GL_TEXTURE0);
    //绑定纹理
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    //加载纹理
    glUniform1i(mTextureLoc,0);
    //加载矩阵
    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,matrix);
    //开启顶点数组缓冲区，第0个
    glEnableVertexAttribArray(ATTRIB_POSITION);
    //参数1：顶点数组索引，参数2：每次取的数量 参数3：数据格式 参数4：是否需要浮点转换 参数5：跨距取值，参数6：保存顶点属性数据的缓冲区指针
    glVertexAttribPointer(ATTRIB_POSITION,VERTEX_POS_SIZE,GL_FLOAT,GL_FALSE,0,VERTICES);
    //开启G顶点数组缓冲区 第1个
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD,TEX_COORD_POS_SZIE,GL_FLOAT,GL_FALSE,0,TEX_COORDS);
    //画方形
    glDrawArrays(GL_TRIANGLE_STRIP,0,VERTEX_NUM);

    //关闭缓冲区
    glDisableVertexAttribArray(ATTRIB_POSITION);
    //关闭缓冲区
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);

    //清空缓冲区，将指令送往硬件立刻执行
    glFlush();
    //缓冲区交换
    mEGLCore->swapBuffer();
}

void CameraEngine::stop() {
    //删除纹理
    glDeleteTextures(1,&mTextureId);
    //停止程序
    glDeleteProgram(mProgram);
    //清空资源
    mEGLCore->release();
}

void CameraEngine::setAssetManager(AAssetManager *assetManager) {
    mAssetManager = assetManager;
}