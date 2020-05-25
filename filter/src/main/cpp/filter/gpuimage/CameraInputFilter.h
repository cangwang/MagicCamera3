//#include <jni.h>
//#include <android/log.h>
//#include <stdio.h>
//#include <malloc.h>
//#include <string.h>
//#include <GLES3/gl3.h>
//#include <android/asset_manager.h>
//#include <string>
//#include "GpuImageFilter.h"
//
//#ifndef _CameraInputFilter
//#define _CameraInputFilter
//
//class CameraInputFilter: public GPUImageFilter{
//
//public:
//    CameraInputFilter();
//    CameraInputFilter(AAssetManager *assetManager);
//    CameraInputFilter(std::string *vertexShader, std::string *fragmentShader);
//    virtual ~CameraInputFilter();
//    void init();
//    void onInputSizeChanged(const int width, const int height);
//    int onDrawFrame(const GLuint textureId, GLfloat *matrix,const float* cubeBuffer, const float* textureBuffer);
//    int onDrawFrame(const GLuint textureId, GLfloat *matrix);
//    GLuint onDrawToTexture(const GLuint textureId,GLfloat *matrix);
//    void destroy();
//    void initCameraFrameBuffer(int width,int height);
//    void destroyCameraFrameBuffers();
//
//protected:
//    virtual void onInit();
//    virtual void onInitialized();
//    bool mIsInitialized;
//
//private:
//    std::string* mVertexShader;
//    std::string* mFragmentShader;
//    float* mGLCubeBuffer;
//    float* mGLTextureBuffer;
//    const int NO_TEXTURE = -1;
//    const int NOT_INIT = -1;
//    const int ON_DRAWN = 1;
//    GLint mMatrixLoc;
//
//    GLuint mFrameBuffer;
//    GLuint mFrameBufferTextures;
//    int mFrameWidth = -1;
//    int mFrameHeight = -1;
//};
//
//#endif