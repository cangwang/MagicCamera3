#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "src/main/cpp/filter/gpuimage/GpuImageFilter.h"
#include "RenderBuffer.h"

/**
 * cangwang 2019.1.4
 */
class MagicVerigoFilter: public GPUImageFilter{

public:
    MagicVerigoFilter();
    MagicVerigoFilter(AAssetManager *assetManager);
    ~MagicVerigoFilter();
    void onDestroy() override ;

protected:
    void onInit() override;
    void onInitialized() override ;
    void onDrawArraysPre() override;
    void onDrawArraysAfter(GLuint textureId) override;

private:
    AAssetManager* mAssetManager;
    GLuint mLutTexture;
    GLint mLutTextureLocation;
    GLuint mLastTexture;
    GLint mLastTextureLocation;
};