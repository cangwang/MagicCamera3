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

#include <algorithm>
#include "GLProgram.hpp"
#include "Context.hpp"
#include "util.h"

NS_GI_BEGIN

std::vector<GLProgram*> GLProgram::_programs;

GLProgram::GLProgram()
:_program(static_cast<GLuint>(-1))
{
    _programs.push_back(this);
}

GLProgram::~GLProgram() {
    auto itr = std::find(_programs.begin(), _programs.end(), this);
    if (itr != _programs.end()) {
        _programs.erase(itr);
    }

    bool bDeleteProgram = (_program != -1);

    for (auto const& program : _programs ) {
        if (bDeleteProgram) {
            if (_program == program->getID()) {
                bDeleteProgram = false;
                break;
            }
        }
    }

    if (bDeleteProgram) {
        glDeleteProgram(_program);
        _program = static_cast<GLuint>(-1);
    }
}

GLProgram* GLProgram::createByShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    auto* ret = new (std::nothrow) GLProgram();
    if (ret) {
        if (!ret->_initWithShaderString(vertexShaderSource, fragmentShaderSource))
        {
            delete ret;
            ret = 0;
        }
    }
    return ret;
}


bool GLProgram::_initWithShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {

    if (_program != static_cast<GLuint>(-1)) {
        CHECK_GL(glDeleteProgram(_program));
        _program = static_cast<GLuint>(-1);
    }
    CHECK_GL(_program = glCreateProgram());

    CHECK_GL(GLuint vertShader = glCreateShader(GL_VERTEX_SHADER));
    const char* vertexShaderSourceStr = vertexShaderSource.c_str();
    CHECK_GL(glShaderSource(vertShader, 1, &vertexShaderSourceStr, NULL));
    CHECK_GL(glCompileShader(vertShader));

    CHECK_GL(GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER));
    const char* fragmentShaderSourceStr = fragmentShaderSource.c_str();
    CHECK_GL(glShaderSource(fragShader, 1, &fragmentShaderSourceStr, NULL));
    CHECK_GL(glCompileShader(fragShader));

    CHECK_GL(glAttachShader(_program, vertShader));
    CHECK_GL(glAttachShader(_program, fragShader));

    CHECK_GL(glLinkProgram(_program));

    CHECK_GL(glDeleteShader(vertShader));
    CHECK_GL(glDeleteShader(fragShader));
    
    return true;
}

GLProgram* GLProgram::_initWithEmptyProgram() {
    auto* ret = new (std::nothrow) GLProgram();
    if (ret) {
        if (!ret->_initProgram())
        {
            delete ret;
            ret = 0;
        }
    }
    return ret;
}

bool GLProgram::_initProgram(){
    if (_program != static_cast<GLuint>(-1)) {
        CHECK_GL(glDeleteProgram(_program));
        _program = static_cast<GLuint>(-1);
    }
    CHECK_GL(_program = glCreateProgram());
    return true;
}

void GLProgram::use() {
    CHECK_GL(glUseProgram(_program));
}

GLuint GLProgram::getAttribLocation(const std::string& attribute) {
    return static_cast<GLuint>(glGetAttribLocation(_program, attribute.c_str()));
}

GLuint GLProgram::getUniformLocation(const std::string& uniformName) {
    return static_cast<GLuint>(glGetUniformLocation(_program, uniformName.c_str()));
}


void GLProgram::setUniformValue(const std::string& uniformName, int value) {
    Context::getInstance()->setActiveShaderProgram(this);
    setUniformValue(getUniformLocation(uniformName), value);
}

void GLProgram::setUniformValue(const std::string& uniformName, float value) {
    Context::getInstance()->setActiveShaderProgram(this);
    setUniformValue(getUniformLocation(uniformName), value);
}

void GLProgram::setUniformValue(const std::string& uniformName, Matrix4 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    setUniformValue(getUniformLocation(uniformName), value);
}

void GLProgram::setUniformValue(const std::string& uniformName, Vector2 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    setUniformValue(getUniformLocation(uniformName), value);
}

void GLProgram::setUniformValue(const std::string& uniformName, Matrix3 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    setUniformValue(getUniformLocation(uniformName), value);
}

void GLProgram::setUniformValue(int uniformLocation, int value) {
    Context::getInstance()->setActiveShaderProgram(this);
    CHECK_GL(glUniform1i(uniformLocation, value));
}

void GLProgram::setUniformValue(int uniformLocation, float value) {
    Context::getInstance()->setActiveShaderProgram(this);
    CHECK_GL(glUniform1f(uniformLocation, value));
}

void GLProgram::setUniformValue(int uniformLocation, Matrix4 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    CHECK_GL(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (GLfloat *)&value));
}

void GLProgram::setUniformValue(int uniformLocation, Vector2 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    CHECK_GL(glUniform2f(uniformLocation, value.x, value.y));
}

void GLProgram::setUniformValue(int uniformLocation, Matrix3 value) {
    Context::getInstance()->setActiveShaderProgram(this);
    CHECK_GL(glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, (GLfloat *)&value));
}

NS_GI_END
