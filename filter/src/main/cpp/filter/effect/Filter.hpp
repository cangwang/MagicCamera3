#ifndef Filter_hpp
#define Filter_hpp

#include "src/main/cpp/filter/source/macros.hpp"
#include "string"
#include "src/main/cpp/filter/source/Source.hpp"
#include "src/main/cpp/filter/source/Target.hpp"
#include "src/main/cpp/filter/source/GLProgram.hpp"
#include "src/main/cpp/filter/source/Ref.hpp"
#include "src/main/cpp/filter/source/util.h"

/**
 * cangwang 2020.3.5
 */
NS_GI_BEGIN

const std::string kDefaultVertexShader = SHADER_STRING(
            attribute vec4 position;
            attribute vec4 texCoord;

            varying vec2 vTexCoord;

            void main(){
                gl_Position = positon;
                vTexCoord = texCoord.xy;
            }
        );

const std::string kDefaultFragmentShader = SHADER_STRING(
            varying highp vec2 vTexCoord;
            uniform sampler2D colorMap;

            void main(){
                gl_FragColor = texture2D(colorMap, vTexCoord);
            }
        );

class Filter : public Source, public Target {
public:
    virtual ~Filter();

    static Filter* create(const std::string& filterClassName);
    static Filter* createWithShaderString(const std::string& vetexShaderSource,
            const std::string& fragmentShaderSource);
    static Filter* createWithFragmentShaderString(const std::string& fragmentShaderSource);

    bool initWithShaderString(const std::string& vetexShaderSource,
            const std::string& fragmentShaderSource);

    virtual bool initWithFragmentShaderString(const std::string& fragmnetShaderSource,
            int inputNumber = 1);
    void setFilterClassName(const std::string filterClassName) {
        _filterClassName = filterClassName;
    }

    std::string getFilterClassName() const {
        return _filterClassName;
    }

    virtual void update(float frameTime) override ;
    virtual bool proceed(bool bUpdateTargets = true) override;
    GLProgram* getProgram() const {
        return _filterProgram;
    }

    bool registerProperty(const std::string& name, int defaultValue,
            const std::string& comment = "",
            std::function<void(int&)> setCallback = nullptr);
    bool registerProperty(const std::string& name, float defaultValue,
            const std::string& comment = "",
            std::function<void(float&)>setCallback = nullptr);
    bool registerProperty(const std::string& name, const std::string& defaultValue,
            const std::string& comment = "",
            std::function<void(std::string&)> setCallback = nullptr);
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
        Registry(const std::string &name, std::function<Filter *()> createFunc) {
            Filter::_registerFilterClass(name, createFunc);
        }
    };

    static void _registerFilterClass(const std::string& filterClassName,
            std::function<Filter*()> createFunc) {
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
    const GLfloat* _getTextureCoordinate(const RotationMode& rotationMode) const ;

    struct Property {
        std::string type;
        std::string comment;
    };
    Property* _getProperty(const std::string& name);

    struct IntProperty : Property {
        int value;
        std::function<void(int&)> setCallback;
    };
    std::map<std::string, IntProperty> _intProperties;

    struct FloatProperty : Property{
        float value;
        std::function<void(float&)> setCallback;
    };
    std::map<std::string, FloatProperty> _floatProperties;

    struct StringProperty : Property{
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
#define  REGISTER_FILTER_CLASS(className)
#endif
NS_GI_END

#endif