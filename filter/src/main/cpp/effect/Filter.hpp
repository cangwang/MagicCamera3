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

#ifndef Filter_hpp
#define Filter_hpp

#include "../source/macros.h"
#include "string"
#include "../source/Source.hpp"
#include "../source/Target.hpp"
#include "../source/GLProgram.hpp"
#include "../source/Ref.hpp"
#include "../source/util.h"

NS_GI_BEGIN


// Hardcode the vertex shader for standard filters, but this can be overridden
    const std::string kDefaultVertexShader = SHADER_STRING
    (
            attribute vec4 position;
            attribute vec4 texCoord;

            varying vec2 vTexCoord;

            void main()
            {
                gl_Position = position;
                vTexCoord = texCoord.xy;
            }
    );

    const std::string kDefaultFragmentShader = SHADER_STRING
    (
            varying highp vec2 vTexCoord;
            uniform sampler2D colorMap;

            void main()
            {
                gl_FragColor = texture2D(colorMap, vTexCoord);
            }
    );

    class Filter : public Source, public Target {
    public:
        virtual ~Filter();

        static Filter* create(const std::string& filterClassName);
        static Filter* createWithShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        static Filter* createWithFragmentShaderString(const std::string& fragmentShaderSource);

        bool initWithShaderString(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        virtual bool initWithFragmentShaderString(const std::string& fragmentShaderSource, int inputNumber = 1);
        void setFilterClassName(const std::string filterClassName) {_filterClassName = filterClassName; }
        std::string getFilterClassName() const { return _filterClassName; };

        virtual void update(float frameTime) override;
        virtual bool proceed(bool bUpdateTargets = true) override;
        GLProgram* getProgram() const { return _filterProgram; };

        // property setters & getters
        bool registerProperty(const std::string& name, int defaultValue, const std::string& comment = "", std::function<void(int&)> setCallback = 0);
        bool registerProperty(const std::string& name, float defaultValue, const std::string& comment = "", std::function<void(float&)> setCallback = 0);
        bool registerProperty(const std::string& name, const std::string& defaultValue, const std::string& comment = "", std::function<void(std::string&)> setCallback = 0);
        bool setProperty(const std::string& name, int value);
        bool setProperty(const std::string& name, float value);
        bool setProperty(const std::string& name, std::string value);
        bool getProperty(const std::string& name, int& retValue);
        bool getProperty(const std::string& name, float& retValue);
        bool getProperty(const std::string& name, std::string& retValue);
        bool hasProperty(const std::string& name);
        bool hasProperty(const std::string& name, const std::string type);
        bool getPropertyComment(const std::string& name, std::string& retComment);
        bool getPropertyType(const std::string& name, std::string& retType);

#if PLATFORM == PLATFORM_ANDROID
        class Registry {
        public:
            Registry(const std::string& name, std::function<Filter*()> createFunc) {
                Filter::_registerFilterClass(name, createFunc);
            }
        };
        static void _registerFilterClass(const std::string& filterClassName, std::function<Filter*()> createFunc) {
            //Log("jin", "Filter：：registerClass : %s", filterClassName.c_str());
            _filterFactories[filterClassName] = createFunc;
        }
#endif

    protected:
        GLProgram* _filterProgram;
        GLuint _filterPositionAttribute;
        std::string _filterClassName;
        struct {
            float r; float g; float b; float a;
        } _backgroundColor;

        Filter();
        std::string _getVertexShaderString() const;
        const GLfloat* _getTexureCoordinate(const RotationMode& rotationMode) const;

        // properties
        struct Property {
            std::string type;
            std::string comment;
        };
        Property* _getProperty(const std::string& name);

        struct IntProperty : Property{
            int value;
            std::function<void(int&)> setCallback;
        };
        std::map<std::string, IntProperty> _intProperties;

        struct FloatProperty : Property {
            float value;
            std::function<void(float&)> setCallback;
        };
        std::map<std::string, FloatProperty> _floatProperties;

        struct StringProperty : Property {
            std::string value;
            std::function<void(std::string&)> setCallback;
        };
        std::map<std::string, StringProperty> _stringProperties;

    private:
        static std::map<std::string, std::function<Filter*()>> _filterFactories;
    };

#if PLATFORM == PLATFORM_ANDROID
#define REGISTER_FILTER_CLASS(className) \
class className##Registry { \
    public: \
        static Filter* newInstance() { \
            return className::create(); \
        } \
    private: \
        static const Filter::Registry _registry; \
}; \
const Filter::Registry className##Registry::_registry(#className, className##Registry::newInstance);
#elif PLATFORM == PLATFORM_IOS
#define REGISTER_FILTER_CLASS(className) 
#endif

NS_GI_END

#endif /* Filter_hpp */
