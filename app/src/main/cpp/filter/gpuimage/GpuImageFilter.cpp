#include "GpuImageFilter.h"
#include "src/main/cpp/utils/OpenglUtils.h"
#include <GLES2/gl2ext.h>
#include <thread>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "src/main/cpp/utils/stb_image_write.h"
#include "src/main/cpp/utils/TextureRotationUtil.h"

#define LOG_TAG "GPUImageFilter"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

/**
 * 滤镜基类
 * cangwang 2018.12.1
 */
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
        mvpMatrix(new float[16]),
        mAssetManager(assetManager),
        mGLCubeBuffer(getCube()),
        mGLTextureBuffer(getRotation(NORMAL, false, false)),
        mScreenWidth(0),mScreenHeight(0),mDisplayWidth(0),mDisplayHeight(0),
        pool(nullptr){
    memcpy(mvpMatrix,NONE_MATRIX,16);
}

GPUImageFilter::~GPUImageFilter() {
    destroyFrameBuffers();
    mGLCubeBuffer = nullptr;
    mGLTextureBuffer = nullptr;
    mAssetManager= nullptr;
    mvpMatrix = nullptr;
    pool = nullptr;
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


    mMatrixLoc = glGetUniformLocation(mGLProgId,"mvpMatrix");
    //初始化成功标志
    mIsInitialized = true;
}

void GPUImageFilter::onInitialized() {

}

void GPUImageFilter::onInputSizeChanged(const int width, const int height) {
    mScreenWidth = width;
    mScreenHeight = height;
    if(mDisplayWidth == 0)
        mDisplayWidth = width;
    if(mDisplayHeight == 0)
        mDisplayHeight = height;
//    initFrameBuffer(width,height);
}

void GPUImageFilter::onInputDisplaySizeChanged(const int width, const int height) {
    mDisplayWidth = width;
    mDisplayHeight = height;
}

void GPUImageFilter::setMvpMatrix(float *mvpMatrix) {
    this->mvpMatrix = mvpMatrix;
}

void GPUImageFilter::setOrientation(int degree) {
    this->degree = degree;
    mGLTextureBuffer = getRotation(degree, false, false);
}

int GPUImageFilter::onDrawFrame(const GLuint textureId,GLfloat *matrix) {
    return onDrawFrame(textureId,matrix,mGLCubeBuffer,mGLTextureBuffer);
}

int GPUImageFilter::onDrawFrameFull(const GLuint textureId,GLfloat *matrix) {
    glViewport(0,0,mScreenWidth,mScreenHeight);
    glUseProgram(mGLProgId);
    if (!mIsInitialized) {
        ALOGE("NOT_INIT");
        return NOT_INIT;
    }

    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,NONE_MATRIX);
    //加载顶点参数
    glVertexAttribPointer(mGLAttribPosition, 2, GL_FLOAT, GL_FALSE, 0, FULL_RECTANGLE_COORDS);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, FULL_RECTANGLE_TEX_COORDS);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        //加载纹理
        glUniform1i(mGLUniformTexture,GL_NONE);
    }
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    //释放顶点绑定
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE) //激活回到默认纹理
        glBindTexture(GL_TEXTURE_2D,GL_NONE);

    return ON_DRAWN;
}

int GPUImageFilter::onDrawFrame(const GLuint textureId, GLfloat *matrix,const float *cubeBuffer,
                                const float *textureBuffer) {
    onDrawPrepare();
    glUseProgram(mGLProgId);
    if (!mIsInitialized) {
        ALOGE("NOT_INIT");
        return NOT_INIT;
    }

    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,NONE_MATRIX);
    //加载顶点参数
    glVertexAttribPointer(mGLAttribPosition, 2, GL_FLOAT, GL_FALSE, 0, cubeBuffer);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, textureBuffer);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        //加载纹理
        glUniform1i(mGLUniformTexture,GL_NONE);
    }
    //滤镜参数加载
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //滤镜参数释放
    onDrawArraysAfter();
    savePictureInThread();
    //释放顶点绑定
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE) //激活回到默认纹理
        glBindTexture(GL_TEXTURE_2D,GL_NONE);

    return ON_DRAWN;
}

GLuint GPUImageFilter::onDrawToTexture(const GLuint textureId,GLfloat *matrix) {
    return onDrawToTexture(textureId,matrix,mGLCubeBuffer,mGLTextureBuffer);
}

GLuint GPUImageFilter::onDrawToTexture(const GLuint textureId, GLfloat *matrix,const float *cubeBuffer,
                                const float *textureBuffer) {
    glViewport(0,0,mFrameWidth,mFrameHeight);
    if(mFrameBuffer>0)
        glBindFramebuffer(GL_FRAMEBUFFER,mFrameBuffer);
    onDrawPrepare();
    glUseProgram(mGLProgId);
    if (!mIsInitialized) {
        ALOGE("NOT_INIT");
        return NOT_INIT;
    }

    glUniformMatrix4fv(mMatrixLoc,1,GL_FALSE,mvpMatrix);
    //加载顶点参数
    glVertexAttribPointer(mGLAttribPosition, 2, GL_FLOAT, GL_FALSE, 0, cubeBuffer);
    glEnableVertexAttribArray(mGLAttribPosition);
    glVertexAttribPointer(mGLAttribTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, textureBuffer);
    glEnableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        //加载纹理
        glUniform1i(mGLUniformTexture,GL_NONE);
    }
    //滤镜参数加载
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //滤镜参数释放
    onDrawArraysAfter();
    drawPixelBuffer();
//    savePictureInThread();
    //释放顶点绑定
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTextureCoordinate);

    if(textureId !=NO_TEXTURE) //激活回到默认纹理
        glBindTexture(GL_TEXTURE_2D,GL_NONE);

    //切换回默认帧缓冲
    if(mFrameBuffer>0)
        glBindFramebuffer(GL_FRAMEBUFFER,GL_NONE);

    return mFrameBufferTextures;
}

void GPUImageFilter::initFrameBuffer(int width, int height) {
    //比对大小
    if (mFrameWidth != width || mFrameHeight !=height){
        destroyFrameBuffers();
    }
    mFrameWidth = width;
    mFrameHeight = height;
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
    glBindTexture(GL_TEXTURE_2D,GL_NONE);
    //切换回默认的帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER,GL_NONE);
    initPixelBuffer(width,height);
}


void GPUImageFilter::destroyFrameBuffers() {
    if(mFrameBufferTextures>0)
        glDeleteTextures(1,&mFrameBufferTextures);
    if(mFrameBuffer>0)
        glDeleteFramebuffers(1,&mFrameBuffer);
    mFrameWidth = -1;
    mFrameHeight = -1;
}

void GPUImageFilter::initPixelBuffer(int width, int height){
    destroyPixelBuffers();
    mPixelBuffer=0;
    int align = 128;//128字节对齐
    mPhoSize = ((width * 4 + (align - 1)) & ~(align - 1))* height;
//    mPhoSize = width*height*4;

    glGenBuffers(1,&mPixelBuffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER,mPixelBuffer);
    glBufferData(GL_PIXEL_PACK_BUFFER,mPhoSize, nullptr,GL_STATIC_READ);

}
void GPUImageFilter::drawPixelBuffer(){
    glBindBuffer(GL_PIXEL_PACK_BUFFER,mPixelBuffer);
    glReadPixels(0,0,mDisplayWidth,mDisplayHeight,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
    mPhoData = (unsigned char*)glMapBufferRange(GL_PIXEL_PACK_BUFFER,0,mPhoSize,GL_MAP_READ_BIT);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    //解除绑定PBO
    glBindBuffer(GL_PIXEL_PACK_BUFFER, GL_NONE);
}

void GPUImageFilter::destroyPixelBuffers() {
    if(mPixelBuffer>0)
        glDeleteTextures(1,&mPixelBuffer);
}

bool GPUImageFilter::savePhoto(std::string saveFileAddress) {
    savePhotoAddress = saveFileAddress;
    isSavePhoto = true;
    return true;
}

void GPUImageFilter::savePictureInThread() {
    if (isSavePhoto && mDisplayWidth > 0 && mDisplayHeight > 0) {
        //加锁
        std::unique_lock<std::mutex> lock(gMutex);
        isSavePhoto = false;
        ALOGV("save address = %s",savePhotoAddress.c_str());
        //字节大小为长*宽*4，RGBA
        long size = mDisplayWidth*mDisplayHeight*4;
        unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char)*size);
        //清空数据
        memset(data,0, sizeof(unsigned char)*size);

        glReadBuffer(GL_FRONT);
        //对齐像素字节
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        checkGLError("glPixelStorei");
        //获取帧内字节
        glReadPixels(0, 0, mDisplayWidth, mDisplayHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //使用线程保存图片
        try {
            if(pool!= nullptr)
                pool->commit(GPUImageFilter::savePicture,savePhotoAddress, data, mDisplayWidth,mDisplayHeight,1);
        }catch (std::exception& e){
            ALOGE("saveImageInThread some unhappy happed,error = %s",e.what());
        }
    }
}

void GPUImageFilter::saveImageInThread(std::string saveFileAddress){
    if (mDisplayWidth > 0 && mDisplayHeight > 0) {
        //加锁
        std::unique_lock<std::mutex> lock(gMutex);
        isSavePhoto = false;
        ALOGV("save address = %s",saveFileAddress.c_str());
//        //字节大小为长*宽*4，RGBA
//        long size = mDisplayWidth*mDisplayHeight*4;
//        unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char)*size);
//        //清空数据
//        memset(data,0, sizeof(unsigned char)*size);
//
//        glReadBuffer(GL_FRONT);
//        checkGLError("glReadBuffer");
//        //对齐像素字节
//        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//        checkGLError("glPixelStorei");
////        glBlitFramebuffer(0,0,mDisplayWidth,mDisplayHeight,0,0,mDisplayWidth,mDisplayHeight,GL_COLOR_BUFFER_BIT,GL_NEAREST);
//        //获取帧内字节
//        glReadPixels(0, 0, mDisplayWidth, mDisplayHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        //使用线程保存图片
//        std::thread thread = std::thread(std::bind(&GPUImageFilter::savePicture, this,saveFileAddress, data, mDisplayWidth,mDisplayHeight,0));
//        std::thread thread = std::thread(std::bind(&GPUImageFilter::savePicture, this,saveFileAddress, mPhoData, mDisplayWidth,mDisplayHeight,0));
//        thread.detach();
        try {
            if(pool!= nullptr)
                pool->commit(GPUImageFilter::savePicture,saveFileAddress, mPhoData, mDisplayWidth,mDisplayHeight,0);
//            pool->commit([=,self=this](){
//               self->savePicture(saveFileAddress, mPhoData, mDisplayWidth,mDisplayHeight,0);
//            });
        }catch (std::exception& e){
            ALOGE("saveImageInThread some unhappy happed,error = %s",e.what());
        }
    }
}

bool GPUImageFilter::savePicture(std::string saveFileAddress,unsigned char* data,int width,int height,int type) {
    //屏幕到文件保存需要使用
    stbi_flip_vertically_on_write(1);
    //保存图片到本地文件
    if (stbi_write_png(saveFileAddress.c_str(), width, height, 4, data, 0)) {
        ALOGV("save address = %s success", saveFileAddress.c_str());
        if(type ==1)
            free(data);
        return true;
    } else {
        if(type ==1)
            free(data);
        ALOGE("save address = %s fail", saveFileAddress.c_str());
        return false;
    };
}

void GPUImageFilter::setPool(std::MagicThreadPool *pool) {
    this->pool = pool;
}

void GPUImageFilter::destroy() {
    mIsInitialized = false;
    glDeleteProgram(mGLProgId);
    onDestroy();
}


