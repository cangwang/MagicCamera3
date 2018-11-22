#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <string>
#include "OpenglUtils.h"
#include "android/asset_manager_jni.h"
#include <fstream>
#include <unistd.h>
#include "src/main/cpp/utils/stb_image.h"

#define LOG_TAG "OpenglUtils"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

//检测错误
void checkGLError(char *op) {
    GLint error = glGetError();
    char err = (char)error;
    if(error!= GL_NO_ERROR){
        std::string msg ="";
        msg.append(op);
        msg.append(":glError 0x");
        msg.append(&err);
//        LOGE(msg.c_str());
    }
}

int loadTextureFromAssets(AAssetManager *manager, const char *fileName){
    GLuint textureHandler=0;
    glGenTextures(1,&textureHandler);
    if (textureHandler!=0){
        glBindTexture(GL_TEXTURE_2D,textureHandler);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        int width,height,n;
        unsigned char* buff = getAddressFromAsset(manager,fileName);
        int size = getSizeFromAsset(manager,fileName);
        unsigned char* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(buff), size, &width, &height, &n, 0);
        ALOGV("loadTextureFromAssets,width = %d,height=%d,n=%d",width,height,n);
        free(buff);
        if(data!=NULL) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else{
            LOGE("load texture from assets is null,fileName = %s",fileName);
        }
        return textureHandler;
    }
    return textureHandler;
}

//BitmapOperation getImageFromAssetsFile(JNIEnv *env, char *filename) {
//
//}
GLuint getExternalOESTextureID(){
    GLuint textureId;
    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,textureId);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    return textureId;
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

/**
 * 获取文件路径
 */

unsigned char* getAddressFromAsset(AAssetManager *manager, const char *fileName){
    //打开asset文件夹
    AAssetDir *dir = AAssetManager_openDir(manager,"");

    const char *file = nullptr;

    while ((file =AAssetDir_getNextFileName(dir))!= nullptr) {
        if (strcmp(file, fileName) == 0) {
            AAsset* asset =AAssetManager_open(manager,file,AASSET_MODE_STREAMING);
            //获取文件长度
            int length = AAsset_getLength(asset);
//            off_t start =0;
//            //获取文件描述符
//            int fd = AAsset_openFileDescriptor(asset,&start,&length);
//            //获取文件路径
//            result = new std::string(getFileAddress(fd));
            unsigned char* buf = (unsigned char *) malloc(sizeof(unsigned char) * length);
            AAsset_read(asset, buf, length);
            AAsset_close(asset);
            return buf;
        }
    }
    AAssetDir_close(dir);
    return NULL;
}

int getSizeFromAsset(AAssetManager *manager, const char *fileName){
    //打开asset文件夹
    AAssetDir *dir = AAssetManager_openDir(manager,"");

    const char *file = nullptr;
    int length =0;
    while ((file =AAssetDir_getNextFileName(dir))!= nullptr) {
        if (strcmp(file, fileName) == 0) {
            AAsset* asset =AAssetManager_open(manager,file,AASSET_MODE_STREAMING);
            //获取文件长度
            length = AAsset_getLength(asset);
            AAsset_close(asset);
            break;
        }
    }
    AAssetDir_close(dir);
    return length;
}


GLuint loadShader(const char *strSource, const int iType) {
    GLint compiled;
    GLuint iShader = glCreateShader(iType);
    if (iShader == 0){
        LOGE("create shader fail");
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
            LOGE("Error compiling shader:[%s]",infoLog);
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
        LOGE("create program failed");
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
            LOGE("loadProgram failed: %s", infoLog);
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
//GLuint getExternalOESTextureID() {
//
//}

char* readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager,
                                               char* fileName) {
    ALOGV("ReadAssets");
    AAssetManager* mgr =AAssetManager_fromJava(env,assetManager);
    if(mgr == NULL){
        LOGE("AAssetManager = null");
        return NULL;
    }

    AAsset* asset = AAssetManager_open(mgr,fileName,AASSET_MODE_UNKNOWN);
    if(asset == NULL){
        LOGE("asset = null");
        return NULL;
    }

    size_t size = AAsset_getLength(asset);
    if (size>0){
        char* pData = (char *)malloc(size+1);
        int numBytesRead = AAsset_read(asset,pData,size);
        LOGE(":%s",pData);
        AAsset_close(asset);
        return pData;
    }
    return NULL;
}

void freeResource(char *pData){
    free(pData);
}
