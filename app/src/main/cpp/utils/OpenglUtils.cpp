#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <string>
#include "OpenglUtils.h"
#include "android/asset_manager_jni.h"

//检测错误
void OpenGlUtils::checkGLError(char *op) {
    GLint error = glGetError();
    char err = (char)error;
    if(error!= GL_NO_ERROR){
        std::string msg ="";
        msg.append(op);
        msg.append(":glError 0x");
        msg.append(&err);
        ALOGE(msg.c_str());
    }
}

BitmapOperation OpenGlUtils::getImageFromAssetsFile(JNIEnv *env, char *filename) {

}

GLuint OpenGlUtils::loadShader(const char *strSource, const int iType) {
    GLint compiled;
    GLuint iShader = glCreateShader(iType);
    glShaderSource(iShader,1,&strSource,NULL);
    glCompileShader(iShader);
    glGetShaderiv(iShader,GL_COMPILE_STATUS,&compiled);
    if(!compiled){
        GLint infoLen = 0;
        glGetShaderiv(iShader,GL_INFO_LOG_LENGTH,&infoLen);

        if(infoLen >1){
            char *infoLog= (char*)malloc(sizeof(char*) *infoLen);
            glGetShaderInfoLog(iShader,infoLen,NULL,infoLog);
            ALOGE("Error compiling shader:[%s]",infoLog);
            free(infoLog);
        }
        glDeleteShader(iShader);
        return 0;
    }
    return iShader;
}

GLuint OpenGlUtils::loadProgram(const char *strVSource, const char *strFSource) {
    GLuint iVShader;
    GLuint iFShader;
    GLuint iProgId;
    GLint lint;
    iVShader = loadShader(strVSource,GL_VERTEX_SHADER);
    if(iVShader == 0){
        ALOGE("Vertex Shader Failed");
        return 0;
    }
    iFShader = loadShader(strFSource,GL_FRAGMENT_SHADER);
    if(iFShader == 0){
        ALOGE("Fragment Shader Failed");
        return 0;
    }

    iProgId = glCreateProgram();
    glAttachShader(iProgId,iVShader);
    glAttachShader(iProgId,iFShader);
    glLinkProgram(iProgId);
    glGetProgramiv(iProgId,GL_LINK_STATUS,&lint);
    if(lint <=0){
        ALOGE("linking Fialed");
        return 0;
    }
    glDeleteShader(iVShader);
    glDeleteShader(iFShader);
    return iProgId;
}

/**
 * 相机使用扩展纹理
 * 只能android传了
 * @return
 */
GLuint OpenGlUtils::getExternalOESTextureID() {

}

char* OpenGlUtils::readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager,
                                               char* fileName) {
    ALOGV("ReadAssets");
    AAssetManager* mgr =AAssetManager_fromJava(env,assetManager);
    if(mgr == NULL){
        ALOGE("AAssetManager = null");
        return NULL;
    }

    AAsset* asset = AAssetManager_open(mgr,fileName,AASSET_MODE_UNKNOWN);
    if(asset == NULL){
        ALOGE("asset = null");
        return NULL;
    }

    size_t size = AAsset_getLength(asset);
    if (size>0){
        char* pData = (char *)malloc(size+1);
        int numBytesRead = AAsset_read(asset,pData,size);
        ALOGE(":%s",pData);
        AAsset_close(asset);
        return pData;
    }
}

void OpenGlUtils::freeResource(char *pData){
    free(pData);
}
