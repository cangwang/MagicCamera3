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
void checkGLError(char *op) {
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

BitmapOperation getImageFromAssetsFile(JNIEnv *env, char *filename) {

}

std::string *readShaderFromAsset(AAssetManager *manager, const char *fileName){

    //打开asset文件夹
    AAssetDir *dir = AAssetManager_openDir(manager,"");
    const char *file = nullptr;
    std::string *result = new std::string;
    while ((file =AAssetDir_getNextFileName(dir))!= nullptr){
        if (strcmp(file,fileName) == 0){
            AAsset *asset = AAssetManager_open(manager,file,AASSET_MODE_STREAMING);
            char buf[1024];
            int nb_read = 0;
            while ((nb_read =AAsset_read(asset,buf, 1024))>0){
                result->append(buf,(unsigned long)nb_read);
            }
            AAsset_close(asset);
            break;
        }
    }
    AAssetDir_close(dir);
    return result;
}

GLuint loadShader(const char *strSource, const int iType) {
    GLint compiled;
    GLuint iShader = glCreateShader(iType);
    if (iShader == 0){
        ALOGE("create shader fail");
        return 0;
    }
    //加载shader源码
    glShaderSource(iShader,1,&strSource,NULL);
    //编译shader
    glCompileShader(iShader);

    //检查编译状态
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

GLuint loadProgram(const char *strVSource, const char *strFSource) {
    GLuint iProgId = glCreateProgram();
    if (iProgId ==0){
        ALOGE("create program failed");
        return 0;
    }
    GLuint iVShader = loadShader(strVSource,GL_VERTEX_SHADER);
    GLuint iFShader = loadShader(strFSource,GL_FRAGMENT_SHADER);
    glAttachShader(iProgId,iVShader);
    glAttachShader(iProgId,iFShader);

    //链接shader到程序
    glLinkProgram(iProgId);

    GLint linked;
    glGetProgramiv(iProgId,GL_LINK_STATUS,&linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(iProgId, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(iProgId, infoLen, nullptr, infoLog);
            ALOGE("loadProgram failed: %s", infoLog);
            free(infoLog);
        }

        glDeleteProgram(iProgId);
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
GLuint getExternalOESTextureID() {

}

char* readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager,
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

void freeResource(char *pData){
    free(pData);
}
