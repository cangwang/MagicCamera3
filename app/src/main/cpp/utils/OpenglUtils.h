#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <string>
#include <android/asset_manager.h>

GLuint loadTextureFromAssets(AAssetManager *manager, const char *fileName);
GLuint loadTextureFromFile(const char *fileName, int *w, int *h,int *n);
GLuint loadTextureFromAssetsRepeat(AAssetManager *manager, const char *fileName);
//BitmapOperation getImageFromAssetsFile(JNIEnv *env,char *filename);
GLuint loadProgram(const char *strVSource, const char *strFSource);
GLuint loadShader(const char *strSource,const int iType);
GLuint getExternalOESTextureID();
GLuint get2DTextureID();
GLuint getLutTextureID();
GLuint get2DTextureRepeatID();
char* readerShaderFromRawResource(JNIEnv *env, jclass tis, jobject assetManager, char* fileName);
//BitmapOperation drawToBitmapByFilter(jobject bitmap,GPUImageFilter filter,int displayWidth,int displayHeight, bool rotate);
void checkGLError(char *op);
void checkGLError(std::string op);
std::string *readShaderFromAsset(AAssetManager *manager, const char *fileName);
unsigned char* getAddressFromAsset(AAssetManager *manager, const char *fileName);
int getSizeFromAsset(AAssetManager *manager, const char *fileName);
unsigned char* getAddressFromAssetFilter(AAssetManager *manager, const char *fileName, int* length);


