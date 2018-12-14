#include "GpuImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES2/gl2ext.h>

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

GPUImageFilter::GPUImageFilter() {

}

GPUImageFilter::GPUImageFilter(AAssetManager *assetManager)
        : GPUImageFilter(assetManager,readShaderFromAsset(assetManager,"nofilter_v.glsl"),readShaderFromAsset(assetManager,"nofilter_f.glsl"))
{

}

GPUImageFilter::GPUImageFilter(AAssetManager *assetManager,std::string *vertexShader, std::string *fragmentShader):
        mVertexShader(vertexShader),
        mFragmentShader(fragmentShader),
        mMatrixLoc(0),
        mAssetManager(assetManager){
    mGLCubeBuffer = CUBE;
    mGLTextureBuffer = getRotation(NORMAL, false, true);
}

GPUImageFilter::~GPUImageFilter() {
    mGLCubeBuffer = nullptr;
    mGLTextureBuffer = nullptr;
    mAssetManager= nullptr;
}

void GPUImageFilter::init() {
    onInit();
    onInitialized();
}

void GPUImageFilter::onInit() {
    mGLProgId = loadProgram(mVertexShader->c_str(),mFragmentShader->c_str());
    //获取顶点着色器
    mGLAttribPosition = glGetAttribLocation(mGLProgId,"position");
    if (mGLAttribPosition<0){
        ALOGE("mGLAttribPosition is illegal");
    }
//    mGLAttribPosition = 0;
    //获取混合顶点着色器
    mGLAttribTextureCoordinate = glGetAttribLocation(mGLProgId,"inputTextureCoordinate");
    if (mGLAttribTextureCoordinate<0){
        ALOGE("mGLAttribTexureCoordinate is illegal");
    }
//    mGLAttribTexureCoordinate = 1;

    //获取纹理统一变量索引
    mGLUniformTexture = glGetUniformLocation(mGLProgId,"inputImageTexture");


//    mMatrixLoc = glGetUniformLocation(mGLProgId,"textureTransform");
    //初始化成功标志
    mIsInitialized = true;
}

void GPUImageFilter::onInitialized() {

}

void GPUImageFilter::onInputSizeChanged(const int width, const int height) {
    mInputWidth = width;
    mInputHeight = height;
}

int GPUImageFilter::onDrawFrame(const GLuint textureId,GLfloat *matrix) {
    return onDrawFrame(textureId,matrix,mGLCubeBuffer,mGLTextureBuffer);
}

int GPUImageFilter::onDrawFrame(const GLuint textureId, GLfloat *matrix,const float *cubeBuffer,
                                const float *textureBuffer) {
    glUseProgram(mGLProgId);
//    runPendingOnDrawTasks()
    if (!mIsInitialized) {
        ALOGE("NOT_INIT");
        return NOT_INIT;
    }
    //加载矩阵
//    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,matrix);
    glVertexAttribPointer(mGLAttribPosition, 2, GL_FLOAT, GL_FALSE, 0, cubeBuffer);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, textureBuffer);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        //加载纹理
        glUniform1i(mGLUniformTexture,0);
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);
    onDrawArraysAfter();
    if(textureId !=NO_TEXTURE)
        glBindTexture(GL_TEXTURE_2D,0);

    return ON_DRAWN;
}

void GPUImageFilter::destroy() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
    onDestroy();
}


