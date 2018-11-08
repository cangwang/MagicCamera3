#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <malloc.h>
#include <GLES3/gl3.h>
#include "gpuimagefilter.h"
#include "src/main/cpp/utils/textureroationutil.h"
#include "src/main/cpp/utils/openglutils.h"


GPUImageFilter::GPUImageFilter(){

}

GPUImageFilter::GPUImageFilter(char *vertexShader, char *fragmentShader) {
    mVertexShader = vertexShader;
    mFragmentShader = fragmentShader;
    mGLCubeBuffer = (float*) malloc(sizeof(TextureRoationUtil::CUBE));
    mGLTextureBuffer = (float*) malloc(sizeof(TextureRoationUtil::TEXTUTRE_NO_ROTATION));
    mGLTextureBuffer[0] = TextureRoationUtil::getRotation(RotationUtil::NORMAL, false, true)[0];
}

void GPUImageFilter::init() {
    onInit();
    onInitialized();
}

void GPUImageFilter::onInit() {
    mGLProgId = OpenGlUtils::loadProgram(mVertexShader,mFragmentShader);
    GLchar *name = const_cast<GLchar *>("position");
    mGLAttribPosition = glGetAttribLocation(mGLProgId,name);
    GLchar *texture = const_cast<GLchar *>("inputImageTexture");
    mGLUniformTexture = glGetUniformLocation(mGLProgId,texture);
    GLchar *coordinate = const_cast<GLchar *>("inputTextureCoordinate");
    mGLAttribTexureCoordinate = glGetAttribLocation(mGLProgId,coordinate);
    mIsInitialized = true;
}

void GPUImageFilter::onInitialized() {

}

void GPUImageFilter::onInputSizeChanged(const int width, const int height) {
    mInputWidth = width;
    mInputHeight = height;
}

int GPUImageFilter::onDrawFrame(const GLint textureId) {
    glUseProgram(mGLProgId);
//    runPendingOnDrawTasks()
    if(!mIsInitialized)
        return OpenGlUtils::NOT_INIT;

    mGLCubeBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribPosition), 2, GL_FLOAT, false, 0, mGLCubeBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribPosition));
    mGLTextureBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribTexureCoordinate), 2, GL_FLOAT, false, 0, mGLTextureBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribTexureCoordinate));
    if(textureId !=OpenGlUtils::NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        glUniform1i(mGLUniformTexture,0);
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTexureCoordinate);
    onDrawArraysAffter();
    glBindTexture(GL_TEXTURE_2D,0);
    return OpenGlUtils::ON_DRAWN;
}

int GPUImageFilter::onDrawFrame(const GLint textureId, const float *cubeBuffer,
                                const float *textureBuffer) {
    glUseProgram(mGLProgId);
//    runPendingOnDrawTasks()
    if(!mIsInitialized)
        return OpenGlUtils::NOT_INIT;

    cubeBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribPosition), 2, GL_FLOAT, false, 0, mGLCubeBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribPosition));
    textureBuffer[0];
    glVertexAttribPointer(static_cast<GLuint>(mGLAttribTexureCoordinate), 2, GL_FLOAT, false, 0, mGLTextureBuffer);
    glEnableVertexAttribArray(static_cast<GLuint>(mGLAttribTexureCoordinate));
    if(textureId !=OpenGlUtils::NO_TEXTURE){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureId);
        glUniform1i(mGLUniformTexture,0);
    }
    onDrawArraysPre();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glDisableVertexAttribArray(mGLAttribPosition);
    glDisableVertexAttribArray(mGLAttribTexureCoordinate);
    onDrawArraysAffter();
    glBindTexture(GL_TEXTURE_2D,0);
    return OpenGlUtils::ON_DRAWN;
}


