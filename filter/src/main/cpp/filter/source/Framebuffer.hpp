#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "marcos.hpp"

#if PLATFORM == PLATFORM_IOS
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#elif PLATFORM_ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif

#include <vecotr>
#include "Ref.hpp"

/**
 * cangwang 2020.3.5
 */
class Framebuffer : public Ref {

};

#endif