#include "GpuImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES2/gl2ext.h>
#include <thread>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "src/main/cpp/utils/stb_image_write.h"

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

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
        mAssetManager(assetManager),
        srcBlend(GL_NONE),
        dstBlend(GL_NONE){
    mGLCubeBuffer = CUBE;
    mGLTextureBuffer = getRotation(NORMAL, false, true);
}

GPUImageFilter::~GPUImageFilter() {
    mGLCubeBuffer = nullptr;
    mGLTextureBuffer = nullptr;
    mAssetManager= nullptr;
    savePhotoAddress = nullptr;
    thread.join();
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

    if (srcBlend != GL_NONE &&dstBlend != GL_NONE){
        //开启颜色混合
        glEnable(GL_BLEND);
        //透明度混合
        glBlendFunc(srcBlend,dstBlend);
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
    onDrawArraysAfter();
    if (isSavePhoto && mInputWidth > 0 && mInputHeight > 0) {
        //加锁
        std::unique_lock<std::mutex> lock(gMutex);
        isSavePhoto = false;
        ALOGV("save address = %s",savePhotoAddress.c_str());
        //字节大小为长*宽*4
        long size = mInputWidth*mInputHeight*4;
        unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char)*size);

        //对齐像素字节
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        //获取帧内字节
        glReadPixels(0, 0, mInputWidth, mInputHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
        thread = std::thread(std::bind(&GPUImageFilter::savePicture, this, data, savePhotoAddress));
    }
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);
//    onDrawArraysAfter();

    if(textureId !=NO_TEXTURE)
        glBindTexture(GL_TEXTURE_2D,0);

    if (srcBlend != GL_NONE &&dstBlend != GL_NONE){
        //关闭颜色混合
       glDisable(GL_BLEND);
    }
    return ON_DRAWN;
}

bool GPUImageFilter::savePhoto(std::string saveFileAddress) {
    savePhotoAddress = saveFileAddress;
    isSavePhoto = true;
    return true;
}

bool GPUImageFilter::savePicture(unsigned char* data,std::string saveFileAddress) {
    //屏幕到文件保存需要使用
    stbi_flip_vertically_on_write(1);
    //保存图片到本地文件
    if (stbi_write_png(saveFileAddress.c_str(), mInputWidth, mInputHeight, 4, data, 0)) {
        ALOGV("save address = %s success", saveFileAddress.c_str());
        free(data);
        return true;
    } else {
        free(data);
        ALOGV("save address = %s fail", saveFileAddress.c_str());
        return false;
    };
}

void GPUImageFilter:: enableBlend(GLenum src,GLenum dst){
    srcBlend = src;
    dstBlend = dst;
}


void GPUImageFilter::destroy() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
    onDestroy();
}


