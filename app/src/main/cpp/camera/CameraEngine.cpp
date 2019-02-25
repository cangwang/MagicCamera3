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

/**
 * cangwang 2018.12.1
 */
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
    if (!mEGLCore->buildContext(mWindow,EGL_NO_CONTEXT)){
        return -1;
    }
    std::string *vShader = readShaderFromAsset(mAssetManager,"camera.vert");
    std::string *fShader = readShaderFromAsset(mAssetManager,"camera.frag");
    //加载程序
    mProgram = loadProgram(vShader->c_str(),fShader->c_str());

    //生成纹理贴图
    glGenTextures(1,&mTextureId);
    //绑定纹理
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    //设置缩小过滤为使用纹理中坐标最接近的一个像素的颜色作为需要绘制的像素颜色，少量计算，渲染比较快，但是效果差
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //设置放大过滤为使用纹理中坐标最接近的若干个颜色，通过加权平均算法得到需要绘制的像素颜色，需要算法计算，用时相对变长，效果好
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //这里GL_TEXTURE_WRAP_S 纹理坐标是以S轴方向与T轴方向纹理（对应平面坐标x，y方向）
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    //初始化矩阵绑定
    mMatrixLoc = glGetUniformLocation(mProgram,"mMatrix");
    //初始化纹理绑定
    mTextureLoc = glGetUniformLocation(mProgram,"sTexture");
    //使用白色清屏
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
    //开启顶点数组缓冲区 第1个
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