#include "GrayscaleFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(GrayscaleFilter)

const std::string kGrayscaleFragmentShaderString = SHADER_STRING
(
   ##version 300 es
   precision highp float;
   uniform sampler2D colorMap;
   in highp vec2 vTexCoord;
   out vec4 glFragColor;

   const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);

   void main()
   {
       lowp vec4 color = texture(colorMap, vTexCoord);
       float luminance = dot(color.rgb, vec3(0.2125, 0.7154, 0.0721));
       gl_FragColor = vec4(vec3(luminance), color.a);
   }
);

GrayscaleFilter* GrayscaleFilter::create() {
    auto ret = new (std::nothrow) GrayscaleFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool GrayscaleFilter::init() {
    return Filter::initWithFragmentShaderString(kGrayscaleFragmentShaderString);
}

bool GrayscaleFilter::proceed(bool updateTargets) {
    return Filter::proceed(updateTargets);
}

NS_GI_END