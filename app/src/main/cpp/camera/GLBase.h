#include <GLES3/gl3.h>

#define LOG_TAG "GLBase"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define ClearRed 0.66f
#define ClearGreen 0.66f
#define ClearBlue 0.66f
#define ClearAlpha 1.0f

class GLBase{
public:
    GLBase():mProgram(0),mWidth(0),mHeight(0){

    }

    void resize(int width,int height){
        mWidth = width;
        mHeight = height;
    }

protected:
    GLuint mProgram;
    GLint mWidth;
    GLint mHeight;

private:

};

