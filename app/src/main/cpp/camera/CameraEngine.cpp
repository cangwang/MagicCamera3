#include <android/native_window.h>
#include <string>
#include "CameraEngine.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES2/gl2ext.h>


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


void CameraEngine::draw() {
    glViewport(0,0,mWidth,mHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(mProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    glUniform1i(mTextureLoc,0);
}

void CameraEngine::stop() {

}