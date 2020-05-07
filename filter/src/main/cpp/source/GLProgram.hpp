#ifndef Shader_hpp
#define Shader_hpp

#include "macros.hpp"
#include "string"
#if PLATFORM == PLATFORM_ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif PLATFORM == PLATFORM_IOS
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#endif

#include <vector>
#include "math.hpp"

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN
class GLProgram {
public:
    GLProgram();
    ~GLProgram();

    static GLProgram* createByShaderString(const std::string& vertexShaderSource,
            const std::string& fragmentShaderSource);
    void use();
    GLuint getID() const {
        return _program;
    }
    GLuint getAttribLocation(const std::string& attribute);
    GLuint getUniformLocation(const std::string& uniformName);

    void setUniformValue(const std::string& uniformName, int value);
    void setUniformValue(const std::string& uniformName, float value);
    void setUniformValue(const std::string& uniformName, Vector2 value);
    void setUniformValue(const std::string& uniformName, Matrix3 value);
    void setUniformValue(const std::string& uniformName, Matrix4 value);

    void setUniformValue(int uniformLocation, int value);
    void setUniformValue(int uniformLocation, float value);
    void setUniformValue(int uniformLocation, Vector2 value);
    void setUniformValue(int uniformLocation, Matrix3 value);
    void setUniformValue(int uniformLocation, Matrix4 value);

private:
    static std::vector<GLProgram*> _programs;
    GLuint _program;
    bool _initWithShaderString(const std::string& vertexShaderSource,
                               const std::string& fragmentShaderSource);
};
NS_GI_END

#endif