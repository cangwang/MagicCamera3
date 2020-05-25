/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef Shader_hpp
#define Shader_hpp

#include "macros.h"
#include "string"
#if PLATFORM == PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif PLATFORM == PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif
#include <vector>
#include "math.hpp"

NS_GI_BEGIN

class GLProgram{
public:
    GLProgram();
    ~GLProgram();
    
    static GLProgram* createByShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    void use();
    GLuint getID() const { return _program; }

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
    bool _initWithShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
};


NS_GI_END

#endif /* GLProgram_hpp */
