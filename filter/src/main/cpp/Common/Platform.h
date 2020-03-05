#ifndef OPENGLES_PLATFORM_H
#define OPENGLES_PLATFORM_H

#define LOG_TAG "GLES"
#define IS_DEBUG TRUE

#if __ANDROID__

#include <GLES3/gl3.h>
#include <android/log.h>

#if IS_DEBUG
#define ESLog(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else
#define ESLog(...)

#endif

#elif __IOS__

#import <OpenGLES/ES3/gl.h>
#import <stdio.h>

#if IS_DEBUG
#define ESLog(fmt, ...) printf("%s: ",LOG_TAG);printf((fmt), ##__VA_ARGS__);printf("\n");

#else
#define ESLog(fmt, ...)

#endif

#endif
