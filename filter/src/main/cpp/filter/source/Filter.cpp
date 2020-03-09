#include "Filter.hpp"
#include "Context.hpp"

/**
 * cangwang 2020.3.5
 */

NS_GI_BEGIN

std::map<std::string, std::function<Filter*()>> Filter::_filterFactories;

Filter::Filter()
:_filterProgram(0)
,_filterClassName("") {
    _backgroundColor.r = 0.0;
    _backgroundColor.g = 0.0;
    _backgroundColor.b = 0.0;
    _backgroundColor.a = 1.0;
}

Filter::~Filter() {
    if (_filterProgram) {
        delete _filterProgram;
        _filterProgram = 0;
    }
}

Filter* Filter::create(const std::string &filterClassName) {
    auto it =_filterFactories.find(filterClassName);
    if (it == _filterFactories.end()) {
        return 0;
    } else {
        Filter* filter = it->second();
        filter->setFilterClassName(filterClassName);
        return it->second();
    }
}

Filter* Filter::createWithShaderString(const std::string &vetexShaderSource,
                                       const std::string &fragmentShaderSource) {
    Filter* filter = new Filter();
    if (!filter->initWithShaderString(vetexShaderSource, fragmentShaderSource)) {
        delete filter;
        filter = 0;
        return 0;
    }
    return filter;
}

Filter* Filter::createWithFragmentShaderString(const std::string &fragmentShaderSource) {
    Filter* filter = new Filter();
    if (!filter->initWithFragmentShaderString(fragmentShaderSource)) {
        delete filter;
        filter = 0;
        return 0;
    }
    return filter;
}

bool Filter::initWithShaderString(const std::string &vetexShaderSource,
                                  const std::string &fragmentShaderSource) {
    _filterProgram = GLProgram::createByShaderString(vetexShaderSource, fragmentShaderSource);
    _filterPositionAttribute = _filterProgram->getAttribLocation("position");
    Context::getInstance()->setActiveShaderProgram(_filterProgram);
    CHECK_GL(glEnableVertexAttribArray(_filterPositionAttribute));

    return true;
}

bool Filter::initWithFragmentShaderString(const std::string &fragmnetShaderSource,
                                          int inputNumber) {
    _inputNum = inputNumber;
    return initWithShaderString(_getVertexShaderString(), fragmnetShaderSource);
}

/**
 * 动态添加参数个数
 * @return
 */
std::string Filter::_getVertexShaderString() const {

    if (_inputNum <= 1)
    {
        return kDefaultVertexShader;
    }

    std::string shaderStr = "\
                attribute vec4 position;\n\
                attribute vec4 texCoord;\n\
                varying vec2 vTexCoord;\n\
                ";
    for (int i = 1; i < _inputNum; ++i) {
        shaderStr += str_format("\
                attribute vec4 texCoord%d;\n\
                varying vec2 vTexCoord%d;\n\
                                ", i, i);
    }
    shaderStr += "\
                void main()\n\
                {\n\
                    gl_Position = position;\n\
                    vTexCoord = texCoord.xy;\n\
        ";
    for (int i = 1; i < _inputNum; ++i) {
        shaderStr += str_format("vTexCoord%d = texCoord%d.xy;\n", i, i);
    }
    shaderStr += "}\n";

    return shaderStr;
}

bool Filter::proceed(bool bUpdateTargets) {
    static const GLfloat imageVertices[] = {
            -1.0f,-1.0f,
            1.0f,-1.0f,
            -1.0f,1.0f,
            1.0f,1.0f,
    };
    Context::getInstance()->setActiveShaderProgram(_filterProgram);
    _framebuffer->active();
    CHECK_GL(glClearColor(_backgroundColor.r, _backgroundColor.g,
            _backgroundColor.b, _backgroundColor.a));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));
    for(auto it = _inputFramebuffers.begin(); it != _inputFramebuffers.end(); ++it) {
        int texIdx = it->first;
        Framebuffer* fb = it->second.framebuffer;
        CHECK_GL(glActiveTexture(GL_TEXTURE0 + texIdx));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, fb->getTexture()));
        _filterProgram->setUniformValue(
                texIdx == 0 ? "colorMap" :str_format("colorMap%d",texIdx),
                texIdx);
        GLuint filterTexCoordAttribute = _filterProgram->getAttribLocation(
                texIdx == 0 ?
                "texCoord" : str_format("texCoord%d",texIdx)
                );
        CHECK_GL(glEnableVertexAttribArray(filterTexCoordAttribute));
        CHECK_GL(glVertexAttribPointer(filterTexCoordAttribute, 2, GL_FLOAT, 0, 0,
                _getTextureCoordinate(it->second.rotationMode)));
    }
    CHECK_GL(glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices));
    CHECK_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    _framebuffer->inactive();
    return Source::proceed(bUpdateTargets);
}

const GLfloat* Filter::_getTextureCoordinate(const RotationMode& rotationMode) const {
        static const GLfloat noRotationTextureCoordinates[] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
        };

        static const GLfloat rotateLeftTextureCoordinates[] = {
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 0.0f,
                0.0f, 1.0f,
        };

        static const GLfloat rotateRightTextureCoordinates[] = {
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
        };

        static const GLfloat verticalFlipTextureCoordinates[] = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                0.0f,  0.0f,
                1.0f,  0.0f,
        };

        static const GLfloat horizontalFlipTextureCoordinates[] = {
                1.0f, 0.0f,
                0.0f, 0.0f,
                1.0f,  1.0f,
                0.0f,  1.0f,
        };

        static const GLfloat rotateRightVerticalFlipTextureCoordinates[] = {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
        };

        static const GLfloat rotateRightHorizontalFlipTextureCoordinates[] = {
                1.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
        };

        static const GLfloat rotate180TextureCoordinates[] = {
                1.0f, 1.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,
        };

        switch (rotationMode) {
            case NoRotation:
                return noRotationTextureCoordinates;
                break;
            case RotateLeft:
                return rotateLeftTextureCoordinates;
                break;
            case RotateRight:
                return rotateRightTextureCoordinates;
                break;
            case FlipVertical:
                return verticalFlipTextureCoordinates;
                break;
            case FlipHorizontal:
                return horizontalFlipTextureCoordinates;
                break;
            case RotateRightFlipVertical:
                return rotateRightVerticalFlipTextureCoordinates;
                break;
            case RotateRightFlipHorizontal:
                return rotateRightHorizontalFlipTextureCoordinates;
                break;
            case Rotate180:
                return rotate180TextureCoordinates;
                break;
            default:
                break;
        }
}

void Filter::update(float frameTime) {
    if (_inputFramebuffers.empty()) return;

    if (Context::getInstance()->isCapturingFrame
        && this == Context::getInstance()->captureUpToFilter) {
        int captureWidth = Context::getInstance()->captureWidth;
        int captureHeight = Context::getInstance()->captureHeight;

        _framebuffer = Context::getInstance()->getFramebufferCache()->fetchFramebuffer
                (captureWidth, captureHeight);
        proceed(false);
        _framebuffer->active();
        Context::getInstance()->capturedFrameData = new unsigned char[captureWidth *
                                                                      captureHeight * 4];
        CHECK_GL(glReadPixels(0, 0, captureWidth, captureHeight, GL_RGBA, GL_UNSIGNED_BYTE,
                Context::getInstance()->capturedFrameData));
        _framebuffer->inactive();
    } else {
        Framebuffer* firstInputFrameBuffer = _inputFramebuffers.begin()->second.framebuffer;
        RotationMode firstInputRotation = _inputFramebuffers.begin()->second.rotationMode;
        if (!firstInputFrameBuffer) return;

        int rotatedFramebufferWidth = firstInputFrameBuffer->getWidth();
        int rotatedFramebufferHeight = firstInputFrameBuffer->getHeight();
        if (rotationSwapsSize(firstInputRotation)) {
            rotatedFramebufferWidth = firstInputFrameBuffer ->getHeight();
            rotatedFramebufferHeight = firstInputFrameBuffer->getWidth();
        }

        if (_framebufferScale != 1.0) {
            rotatedFramebufferWidth = int(rotatedFramebufferWidth * _framebufferScale);
            rotatedFramebufferHeight = int(rotatedFramebufferHeight * _framebufferScale);
        }

        _framebuffer = Context::getInstance()->getFramebufferCache()->fetchFramebuffer
                (rotatedFramebufferWidth,rotatedFramebufferHeight);
        proceed();
    }

    _framebuffer->release();
    _framebuffer = 0;
}


bool Filter::registerProperty(const std::string& name, int defaultValue, const std::string& comment/* = ""*/, std::function<void(int&)> setCallback/* = 0*/) {
        if (hasProperty(name)) return false;
        IntProperty property;
        property.type = "int";
        property.value = defaultValue;
        property.comment = comment;
        property.setCallback = setCallback;
        _intProperties[name] = property;
        return true;
}

bool Filter::registerProperty(const std::string& name, float defaultValue, const std::string& comment/* = ""*/, std::function<void(float&)> setCallback/* = 0*/) {
        if (hasProperty(name)) return false;
        FloatProperty property;
        property.type = "float";
        property.value = defaultValue;
        property.comment = comment;
        property.setCallback = setCallback;
        _floatProperties[name] = property;
        return true;
}

bool Filter::registerProperty(const std::string& name, const std::string& defaultValue, const std::string& comment/* = ""*/, std::function<void(std::string&)> setCallback/* = 0*/) {
        if (hasProperty(name)) return false;
        StringProperty property;
        property.type = "string";
        property.value = defaultValue;
        property.comment = comment;
        property.setCallback = setCallback;
        _stringProperties[name] = property;
        return true;
}

    bool Filter::setProperty(const std::string& name, int value) {
        Property* rawProperty = _getProperty(name);
        if (!rawProperty) {
            Log("WARNING", "Filter::setProperty invalid property %s", name.c_str());
            return false;
        } else if (rawProperty->type != "int") {
            Log("WARNING", "Filter::setProperty The property type is expected to be %s", rawProperty->type.c_str());
            return false;
        }
        IntProperty* property = ((IntProperty*)rawProperty);
        property->value = value;
        if (property->setCallback)
            property->setCallback(value);
        return true;
    }

    bool Filter::setProperty(const std::string& name, float value) {
        Property* rawProperty = _getProperty(name);
        if (!rawProperty) {
            Log("WARNING", "Filter::setProperty invalid property %s", name.c_str());
            return false;
        } else if (rawProperty->type != "float") {
            Log("WARNING", "Filter::setProperty The property type is expected to be %s", rawProperty->type.c_str());
            return false;
        }
        FloatProperty* property = ((FloatProperty*)rawProperty);
        if (property->setCallback)
            property->setCallback(value);
        property->value = value;

        return true;
    }

    bool Filter::setProperty(const std::string& name, std::string value) {
        Property* rawProperty = _getProperty(name);
        if (!rawProperty) {
            Log("WARNING", "Filter::setProperty invalid property %s", name.c_str());
            return false;
        } else if (rawProperty->type != "string") {
            Log("WARNING", "Filter::setProperty The property type is expected to be %s", rawProperty->type.c_str());
            return false;
        }
        StringProperty* property = ((StringProperty*)rawProperty);
        property->value = value;
        if (property->setCallback)
            property->setCallback(value);
        return true;
    }

    bool Filter::getProperty(const std::string& name, int& retValue) {
        Property* property = _getProperty(name);
        if (!property) return false;
        retValue = ((IntProperty*)property)->value;
        return true;
    }

    bool Filter::getProperty(const std::string& name, float& retValue) {
        Property* property = _getProperty(name);
        if (!property) return false;
        retValue = ((FloatProperty*)property)->value;
        return true;
    }

    bool Filter::getProperty(const std::string& name, std::string& retValue) {
        Property* property = _getProperty(name);
        if (!property) return false;
        retValue = ((StringProperty*)property)->value;
        return true;
    }

bool Filter::hasProperty(const std::string& name) {
        return _getProperty(name) ? true : false;
}

bool Filter::hasProperty(const std::string& name, const std::string type) {
        Property* property = _getProperty(name);
        return property && property->type == type ? true : false;
}

    bool Filter::getPropertyComment(const std::string& name, std::string& retComment) {
        Property* property = _getProperty(name);
        if (!property) return false;
        retComment = std::string("[") + property->type + "] " + property->comment;
        return true;
    }

    bool Filter::getPropertyType(const std::string& name, std::string& retType) {
        Property* property = _getProperty(name);
        if (!property) return false;
        retType = property->type;
        return true;
    }

    Filter::Property* Filter::_getProperty(const std::string& name) {
        if (_intProperties.find(name) != _intProperties.end()) {
            return &_intProperties[name];
        }
        if (_floatProperties.find(name) != _floatProperties.end()) {
            return &_floatProperties[name];
        }
        if (_stringProperties.find(name) != _stringProperties.end()) {
            return &_stringProperties[name];
        }
        return 0;
    }

NS_GI_END