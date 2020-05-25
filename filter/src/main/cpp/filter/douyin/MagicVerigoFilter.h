#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "filter/gpuimage/GpuImageFilter.h"
#include "RenderBuffer.h"
#include <utils/TextureRotationUtil.h>

/**
 * cangwang 2019.1.4
 */
class MagicVerigoFilter: public GPUImageFilter{

public:
    MagicVerigoFilter();
    MagicVerigoFilter(AAssetManager *assetManager);
    void onInputSizeChanged(const int width, const int height) override ;
    ~MagicVerigoFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override;
    void onDrawPrepare() override;
    void onDrawArraysAfter() override;
//    void onDrawArraysAfter(GLuint textureId) override;

private:
    AAssetManager* mAssetManager;
//    GLuint mLutTexture;
//    GLint mLutTextureLocation;
//    GLuint mLastTexture;
//    GLint mLastTextureLocation;

    RenderBuffer* mRenderBuffer;
    RenderBuffer* mRenderBuffer2;
    RenderBuffer* mRenderBuffer3;
    GLint mLutTexture;
    GLuint mCurrentFrameProgram;
    GLuint mLastFrameProgram;
    bool mFirst = true;
    void drawToBuffer();
    void drawCurrentFrame();
    void setup(GLuint programId,GLint* textureId);
};