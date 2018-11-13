#include "GpuImageFilter.h"
#include "src/main/cpp/utils/TextureRotationUtil.h"
#include "src/main/cpp/utils/OpenglUtils.h"

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

GPUImageFilter::GPUImageFilter(){

}

GPUImageFilter::GPUImageFilter(char *vertexShader, char *fragmentShader) {
    mVertexShader = vertexShader;
    mFragmentShader = fragmentShader;
    mGLCubeBuffer = (float*) malloc(sizeof(TextureRotationUtil::CUBE));
    mGLTextureBuffer = (float*) malloc(sizeof(TextureRotationUtil::TEXTUTRE_NO_ROTATION));
    mGLTextureBuffer[0] = TextureRotationUtil::getRotation(RotationUtil::NORMAL, false, true)[0];
}

void GPUImageFilter::init() {
    onInit();
    onInitialized();
}

void GPUImageFilter::onInit() {
    mGLProgId = loadProgram(mVertexShader,mFragmentShader);
    GLchar *name = const_cast<GLchar *>("position");
    //获取顶点着色器
    mGLAttribPosition = glGetAttribLocation(mGLProgId,name);
    GLchar *texture = const_cast<GLchar *>("inputImageTexture");
    //获取纹理统一变量索引
    mGLUniformTexture = glGetUniformLocation(mGLProgId,texture);
    GLchar *coordinate = const_cast<GLchar *>("inputTextureCoordinate");
    //获取混合顶点着色器
    mGLAttribTexureCoordinate = glGetAttribLocation(mGLProgId,coordinate);
    //初始化成功标志
    mIsInitialized = true;
}

void GPUImageFilter::onInitialized() {

}

void GPUImageFilter::onInputSizeChanged(const int width, const int height) {
    mInputWidth = width;
    mInputHeight = height;
}

int GPUImageFilter::onDrawFrame(const GLint textureId) {
    //加载着色器程序
    glUseProgram(mGLProgId);
//    runPendingOnDrawTasks()
    if(!mIsInitialized) //未初始化则返回
        return NOT_INIT;

    mGLCubeBuffer[0];
    //从Cube中一次取两个Float值，下次偏移为0
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribPosition), 2, GL_FLOAT, false, 0, mGLCubeBuffer);
    //开启通用顶点属性数据
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribPosition));
    mGLTextureBuffer[0];
    //从纹理中一次取两个Float值，下次偏移为0
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribTexureCoordinate), 2, GL_FLOAT, false, 0, mGLTextureBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribTexureCoordinate));
    if(textureId !=NO_TEXTURE){ //如果纹理id不为-1，则纹理存在
        glActiveTexture(GL_TEXTURE0);  //初始化纹理必须调用
        glBindTexture(GL_TEXTURE_2D,textureId); //绑定2D纹理
        glUniform1i(mGLUniformTexture,0); //纹理采样
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4); //绘制连接的三角形（4个点为四边形）
    glDisableVertexAttribArray(mGLAttribPosition); //关闭顶点属性数据
    glDisableVertexAttribArray(mGLAttribTexureCoordinate); //关闭纹理顶点属性数据
    onDrawArraysAfter();
    glBindTexture(GL_TEXTURE_2D,0); //解除纹理绑定
    return ON_DRAWN;  //返回绘画成功
}

int GPUImageFilter::onDrawFrame(const GLint textureId, const float *cubeBuffer,
                                const float *textureBuffer) {
    glUseProgram(mGLProgId);
//    runPendingOnDrawTasks()
    if(!mIsInitialized)
        return NOT_INIT;

    cubeBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribPosition), 2, GL_FLOAT, false, 0, mGLCubeBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribPosition));
    textureBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribTexureCoordinate), 2, GL_FLOAT, false, 0, mGLTextureBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribTexureCoordinate));
    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        glUniform1i(mGLUniformTexture,0);
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTexureCoordinate);
    onDrawArraysAfter();
    glBindTexture(GL_TEXTURE_2D,0);
    return ON_DRAWN;
}


