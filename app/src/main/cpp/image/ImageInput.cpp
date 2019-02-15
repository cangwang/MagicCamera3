#include "src/main/cpp/utils/OpenglUtils.h"
#include "ImageInput.h"
#include <GLES2/gl2ext.h>
#include <src/main/cpp/utils/TextureRotationUtil.h>

#define LOG_TAG "ImageInput"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

ImageInput::ImageInput() {

}

ImageInput::ImageInput(AAssetManager *assetManager,std::string path):
        mVertexShader(readShaderFromAsset(assetManager,"default_vertex.glsl")),
        mFragmentShader(readShaderFromAsset(assetManager,"default_none_fragment.glsl")),
        imgPath(path),
        mGLCubeBuffer(getCube()),
        mGLTextureBuffer(getRotation(NORMAL, false, true)){

}


ImageInput::~ImageInput() {
    mGLCubeBuffer = nullptr;
    mGLTextureBuffer = nullptr;
}

void ImageInput::init() {
    onInit();
    onInitialized();
}

void ImageInput::onInit() {
    mGLProgId = loadProgram(mVertexShader->c_str(),mFragmentShader->c_str());
    //获取顶点着色器
    mGLAttribPosition = glGetAttribLocation(mGLProgId,"position");
    if (mGLAttribPosition <0){
        ALOGE("mGLAttribPosition is illegal.");
    }
    //获取混合顶点着色器
    mGLAttribTextureCoordinate = glGetAttribLocation(mGLProgId,"inputTextureCoordinate");
    if (mGLAttribTextureCoordinate < 0){
        ALOGE("mGLAttribTexureCoordinate is illegal.");
    }

    //获取纹理统一变量索引
    mGLUniformTexture = glGetUniformLocation(mGLProgId,"inputImageTexture");


//    mMatrixLoc = glGetUniformLocation(mGLProgId,"textureTransform");
    //初始化成功标志
    mIsInitialized = true;

    mTexturetransformMatrixlocation = glGetUniformLocation(mGLProgId,"textureTransform");
    mSingleStepOffsetLocation = glGetUniformLocation(mGLProgId,"singleStepOffset");
    mParamsLocation = glGetUniformLocation(mGLProgId,"params");
}

void ImageInput::onInitialized() {

}

void ImageInput::onInputSizeChanged(const int width, const int height) {
    mScreenWidth = width;
    mScreenHeight = height;
    if(imgTexture>0){
        glDeleteTextures(1,&imgTexture);
    }
    imgTexture = loadTextureFromFile(imgPath.c_str(),&mImageWidth,&mImageHeight);
}

int ImageInput::onDrawFrame(const GLuint textureId,GLfloat *matrix) {
    return onDrawFrame(textureId,matrix,mGLCubeBuffer,mGLTextureBuffer);
}

int ImageInput::onDrawFrame(const GLuint textureId, GLfloat *matrix,const float *cubeBuffer,
                                const float *textureBuffer) {
    glUseProgram(mGLProgId);
    if(!mIsInitialized)
        return NOT_INIT;
    //加载矩阵
//    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,matrix);
    glVertexAttribPointer(mGLAttribPosition, 2, GL_FLOAT, GL_FALSE, 0, cubeBuffer);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, textureBuffer);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);
    glUniformMatrix4fv(mTexturetransformMatrixlocation,1, GL_FALSE,matrix);

    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        //加载纹理
        glUniform1i(mGLUniformTexture,0);
    }

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);
    glBindTexture(GL_TEXTURE_2D,0);

    return ON_DRAWN;
}

GLuint ImageInput::onDrawToTexture(const GLuint textureId, GLfloat *matrix) {
    //视口切换
    glViewport(0,0,mImageWidth,mImageHeight);
    //绑定帧缓冲id
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBuffer);
    glUseProgram(mGLProgId);
    if (!mIsInitialized){
        return (GLuint) NOT_INIT;
    }
    //顶点缓冲
    glVertexAttribPointer(mGLAttribPosition,2,GL_FLOAT,GL_FALSE,0,mGLCubeBuffer);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate,2,GL_FLOAT,GL_FALSE,0,mGLTextureBuffer);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);
    glUniformMatrix4fv(mTexturetransformMatrixlocation,1,GL_FALSE,matrix);
    //设置美颜等级
    setBeautyLevelOnDraw(beautyLevel);
    setTexelSize(mScreenWidth,mScreenHeight);
    //加载矩阵
//    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,matrix);

    if (textureId != NO_TEXTURE){
        //绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        glUniform1i(mGLUniformTexture,0);
    }
    //绘制图像（长方形）
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //关闭顶点缓冲
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);
    //切换回默认纹理
    glBindTexture(GL_TEXTURE_2D,0);
    //切换回默认帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    return mFrameBufferTextures;
}

void ImageInput::destroy() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
    onDestroy();
}

void ImageInput::initFrameBuffer(int width, int height) {
    //比对大小
    if ( mImageWidth != width || mImageHeight !=height){
        destroyFrameBuffers();
    }
    mImageWidth = width;
    mImageHeight = height;
    mFrameBuffer=0;
    mFrameBufferTextures=0;
    //生成帧缓冲id
    glGenFramebuffers(1,&mFrameBuffer);
    //生成纹理id
    glGenTextures(1,&mFrameBufferTextures);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D,mFrameBufferTextures);
    //纹理赋值为空，先纹理占位
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, nullptr);
    //设定纹理参数
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    //绑定帧图
    glBindFramebuffer(GL_FRAMEBUFFER,mFrameBuffer);
    //绑定纹理到帧图
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,mFrameBufferTextures,0);
    //切换回默认纹理
    glBindTexture(GL_TEXTURE_2D,0);
    //切换回默认的帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void ImageInput::destroyFrameBuffers() {
    glDeleteTextures(1,&mFrameBufferTextures);
    glDeleteFramebuffers(1,&mFrameBuffer);
    mImageWidth = -1;
    mImageHeight = -1;
}

void ImageInput::setTexelSize(int width,int height){
    glUniform2f(mSingleStepOffsetLocation,2.0f/width,2.0f/height);
}

void ImageInput::setBeautyLevel(int level){
    ALOGV("beauty Level = %d",level);
    beautyLevel = level;
}

void ImageInput::setBeautyLevelOnDraw(int level){
//    ALOGV("setbeautyLevel = %d",level);
    switch (level){
        case 0:
            glUniform1f(mParamsLocation,0.0f);
            break;
        case 1:
            glUniform1f(mParamsLocation,1.0f);
            break;
        case 2:
            glUniform1f(mParamsLocation,0.8f);
            break;
        case 3:
            glUniform1f(mParamsLocation,0.6f);
            break;
        case 4:
            glUniform1f(mParamsLocation,0.4f);
            break;
        case 5:
            glUniform1f(mParamsLocation,0.33f);
            break;
        default:
            break;
    }
}



