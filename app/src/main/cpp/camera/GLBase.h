#include <GLES3/gl3.h>

#define ClearRed 0.66f
#define ClearGreen 0.66f
#define ClearBlue 0.66f
#define ClearAlpha 1.0f

#ifndef _GLBase
#define _GLBase

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

#endif
