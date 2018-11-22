#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <string>
#include <android/asset_manager.h>

int loadTexture(const char *img,const int userTexId);
int loadTexture(const char *img,const int userTexId, bool recyled);
int loadTexture(const char data[],const int width,const int height,const int userTexId);
int loadTexture(const char data[],const int width,const int height,const int userTexId,int type);
int loadTexture(const JNIEnv *env,const char* name);
//BitmapOperation getImageFromAssetsFile(JNIEnv *env,char *filename);
GLuint loadProgram(const char *strVSource, const char *strFSource);
GLuint loadShader(const char *strSource,const int iType);
GLuint getExternalOESTextureID();
char* readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager, char* fileName);
//BitmapOperation drawToBitmapByFilter(jobject bitmap,GPUImageFilter filter,int displayWidth,int displayHeight, bool rotate);
void checkGLError(char *op);
void freeResource(char *pData);
std::string *readShaderFromAsset(AAssetManager *manager, const char *fileName);
std::string *getAddressFromAsset(AAssetManager *manager, const char *fileName)
std::string getFileAddress(const int fd);


