#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <src/main/cpp/bitmap/BitmapOperation.h>
#include "src/main/cpp/utils/OpenglUtils.h"
#include <android/asset_manager.h>
#include "EGLCore.h"
#include "GLBase.h"

class CameraEngine:public GLBase{
public:
    CameraEngine(ANativeWindow *window);
    ~CameraEngine();
    void setAssetManager(AAssetManager *assetManager);
    int create();
    void draw(GLfloat *matrix);
    void stop();

protected:

private:
    AAssetManager *mAssetManager;
    ANativeWindow *mWindow;
    GLuint mTextureId;
    GLint mTextureLoc;
    GLint mMatrixLoc;
    GLfloat mMatrix[16];
    EGLCore *mEGLCore;
};

