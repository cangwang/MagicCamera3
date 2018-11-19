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

const static GLuint ATTRIB_POSITION = 0;
const static GLuint ATTRIB_TEXCOORD = 1;

GPUImageFilter::GPUImageFilter() {

}

GPUImageFilter::GPUImageFilter(AAssetManager *assetManager){
    GPUImageFilter(readShaderFromAsset(assetManager,"nofilter_v.glsl"),readShaderFromAsset(assetManager,"nofilter_f.glsl"));
}

GPUImageFilter::GPUImageFilter(std::string *vertexShader, std::string *fragmentShader):mVertexShader(vertexShader),mFragmentShader(fragmentShader) {
    mGLCubeBuffer = (float*) malloc(sizeof(CUBE));
    mGLTextureBuffer = (float*) malloc(sizeof(TEXTUTRE_NO_ROTATION));
    mGLTextureBuffer[0] = TextureRotationUtil::getRotation(NORMAL, false, true)[0];
}

GPUImageFilter::~GPUImageFilter() {

}

void GPUImageFilter::init() {
    onInit();
    onInitialized();
}

void GPUImageFilter::onInit() {
    mGLProgId = loadProgram(mVertexShader->c_str(),mFragmentShader->c_str());
    //获取顶点着色器
    mGLAttribPosition = glGetAttribLocation(mGLProgId,"position");
    //获取纹理统一变量索引
    mGLUniformTexture = glGetUniformLocation(mGLProgId,"inputImageTexture");
    //获取混合顶点着色器
    mGLAttribTexureCoordinate = glGetAttribLocation(mGLProgId,"inputTextureCoordinate");
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

    //从Cube中一次取两个Float值，下次偏移为0
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, mGLCubeBuffer);
    //开启通用顶点属性数据
    glEnableVertexAttribArray(ATTRIB_POSITION);
    //从纹理中一次取两个Float值，下次偏移为0
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, mGLTextureBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribTexureCoordinate));
    if(textureId !=NO_TEXTURE){ //如果纹理id不为-1，则纹理存在
        glActiveTexture(GL_TEXTURE0);  //初始化纹理必须调用
        glBindTexture(GL_TEXTURE_2D,textureId); //绑定2D纹理
        glUniform1i(mGLUniformTexture,0); //纹理采样
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4); //绘制连接的三角形（4个点为四边形）
    glDisableVertexAttribArray(ATTRIB_POSITION); //关闭顶点属性数据
    glDisableVertexAttribArray(ATTRIB_TEXCOORD); //关闭纹理顶点属性数据
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

    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, cubeBuffer);
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, textureBuffer);
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        glUniform1i(mGLUniformTexture,0);
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(ATTRIB_POSITION);
    glDisableVertexAttribArray(ATTRIB_TEXCOORD);
    onDrawArraysAfter();
    glBindTexture(GL_TEXTURE_2D,0);
    return ON_DRAWN;
}

void GPUImageFilter::destroy() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
    onDestroy();
}


