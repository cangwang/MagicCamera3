#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <bitmap/BitmapOperation.h>
#include "utils/OpenglUtils.h"
#include <android/asset_manager.h>
#include "egl/EGLCore.h"
#include "egl/GLBase.h"

/**
 * cangwang 2018.12.1
 */
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

