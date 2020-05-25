#if PLATFORM == PLATFORM_ANDROID

#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "src/main/cpp/source/SourceImage.h"
#include "src/main/cpp/source/SourceCamera.h"
#include "src/main/cpp/source/TargetView.h"
#include "src/main/cpp/effect/Filter.hpp"
#include "src/main/cpp/source/Context.hpp"

USING_NS_GI

extern "C" {
JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceImageNew(
        JNIEnv *env,
        jclass obj)
{
    return (uintptr_t)(new SourceImage());
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceImageDestroy(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((SourceImage*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceImageFinalize(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((SourceImage*)classId)->releaseFramebuffer(false);
    ((SourceImage*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceImageSetImage(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jobject bitmap)
{
    char* pData = 0;
    AndroidBitmapInfo info;
    void* pixels;
    if ((AndroidBitmap_getInfo(env, bitmap, &info)) < 0){
        //ERROR
        return;
    }

    if ((AndroidBitmap_lockPixels(env, bitmap, &pixels)) >= 0){
        ((SourceImage*)classId)->setImage(info.width, info.height, pixels);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
};

JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceCameraNew(
        JNIEnv *env,
        jclass obj)
{
    return (uintptr_t)(new SourceCamera());
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceCameraDestroy(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((SourceCamera*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceCameraFinalize(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((SourceCamera*)classId)->releaseFramebuffer(false);
    ((SourceCamera*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceCameraSetFrame(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jint width,
        jint height,
        jintArray jdata,
        jint rotation)
{
    jint* data = (jint*) (env->GetPrimitiveArrayCritical(jdata, 0));
    ((SourceCamera*)classId)->setFrameData(width, height, data, (RotationMode)rotation);
    env->ReleasePrimitiveArrayCritical(jdata, data, 0);
};

JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceAddTarget(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jlong targetClassId,
        jint texID,
        jboolean isFilter)
{
    Source* source = (Source *) classId;
    Target* target = isFilter ? dynamic_cast<Target*>((Filter*)targetClassId) : (Target*)targetClassId;
    if (texID >= 0) {
        return (uintptr_t) (source->addTarget(target, texID));
    } else {
        return (uintptr_t) (source->addTarget(target));
    }
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceRemoveTarget(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jlong targetClassId,
        jboolean isFilter)
{
    Source* source = (Source *) classId;
    Target* target = isFilter ? dynamic_cast<Target*>((Filter*)targetClassId) : (Target*)targetClassId;
    source->removeTarget(target);
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceRemoveAllTargets(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((Source *) classId)->removeAllTargets();
};

JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceProceed(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jboolean bUpdateTargets )
{
    return ((Source *) classId)->proceed(bUpdateTargets);
};

JNIEXPORT jint JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceGetRotatedFramebuferWidth(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    return ((Source *) classId)->getRotatedFramebufferWidth();
};

JNIEXPORT jint JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceGetRotatedFramebuferHeight(
        JNIEnv *env,
        jclass,
        jlong classId)
{
    return ((Source *) classId)->getRotatedFramebufferHeight();
};

JNIEXPORT jbyteArray JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeSourceCaptureAProcessedFrameData(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jlong upToFilterClassId,
        jint width,
        jint height )
{
    unsigned char* processedFrameData = ((Source *) classId)->captureAProcessedFrameData((Filter*)upToFilterClassId, width, height);
    int frameSize = width * height * 4 * sizeof(unsigned char);

    jbyteArray jresult = NULL;
    if (processedFrameData) {
        jbyte* by = (jbyte*)processedFrameData;
        jresult = env->NewByteArray(frameSize);
        env->SetByteArrayRegion(jresult, 0, frameSize, by);
        delete[] processedFrameData;
        processedFrameData = 0;
    }

    return jresult;
};

JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeTargetViewNew(
        JNIEnv *env,
        jclass obj)
{
    return (uintptr_t)(new TargetView());
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeTargetViewFinalize(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((TargetView*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeTargetViewOnSizeChanged(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jint width,
        jint height)
{
    ((TargetView*)classId)->onSizeChanged(width, height);
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeTargetViewSetFillMode(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jint fillMode)
{
    ((TargetView*)classId)->setFillMode((TargetView::FillMode)fillMode);
};

JNIEXPORT jlong JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterCreate(
        JNIEnv *env,
        jclass obj,
        jstring jFilterClassName)
{
    const char* filterClassName = env->GetStringUTFChars(jFilterClassName, 0);
    long ret = (uintptr_t)(Filter::create(filterClassName));
    env->ReleaseStringUTFChars(jFilterClassName, filterClassName);
    return ret;
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterDestroy(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((Filter*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterFinalize(
        JNIEnv *env,
        jclass obj,
        jlong classId)
{
    ((Filter*)classId)->releaseFramebuffer(false);
    ((Filter*)classId)->release();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterSetPropertyFloat(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jstring jProperty,
        jfloat value)
{
    const char* property = env->GetStringUTFChars(jProperty, 0);
    ((Filter*)classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);

};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterSetPropertyInt(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jstring jProperty,
        jint value)
{
    const char* property = env->GetStringUTFChars(jProperty, 0);
    ((Filter*)classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);

};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeFilterSetPropertyString(
        JNIEnv *env,
        jclass obj,
        jlong classId,
        jstring jProperty,
        jstring jValue)
{
    const char* property = env->GetStringUTFChars(jProperty, 0);
    const char* value = env->GetStringUTFChars(jValue, 0);
    ((Filter*)classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);
    env->ReleaseStringUTFChars(jValue, value);

};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeContextInit(
        JNIEnv *env,
        jclass obj)
{
    Context::init();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeContextDestroy(
        JNIEnv *env,
        jclass obj)
{
    Context::destroy();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeContextPurge(
        JNIEnv *env,
        jclass obj)
{
    Context::getInstance()->purge();
};

JNIEXPORT void JNICALL
Java_com_cangwang_gpuimage_GPUImage_nativeYUVtoRBGA(JNIEnv * env, jclass obj, jbyteArray yuv420sp, jint width, jint height, jintArray rgbOut)
{
    int             sz;
    int             i;
    int             j;
    int             Y;
    int             Cr = 0;
    int             Cb = 0;
    int             pixPtr = 0;
    int             jDiv2 = 0;
    int             R = 0;
    int             G = 0;
    int             B = 0;
    int             cOff;
    int w = width;
    int h = height;
    sz = w * h;

    jint *rgbData = (jint*) (env->GetPrimitiveArrayCritical(rgbOut, 0));
    jbyte* yuv = (jbyte*) env->GetPrimitiveArrayCritical(yuv420sp, 0);

    for(j = 0; j < h; j++) {
        pixPtr = j * w;
        jDiv2 = j >> 1;
        for(i = 0; i < w; i++) {
            Y = yuv[pixPtr];
            if(Y < 0) Y += 255;
            if((i & 0x1) != 1) {
                cOff = sz + jDiv2 * w + (i >> 1) * 2;
                Cb = yuv[cOff];
                if(Cb < 0) Cb += 127; else Cb -= 128;
                Cr = yuv[cOff + 1];
                if(Cr < 0) Cr += 127; else Cr -= 128;
            }

            //ITU-R BT.601 conversion
            //
            //R = 1.164*(Y-16) + 2.018*(Cr-128);
            //G = 1.164*(Y-16) - 0.813*(Cb-128) - 0.391*(Cr-128);
            //B = 1.164*(Y-16) + 1.596*(Cb-128);
            //
            Y = Y + (Y >> 3) + (Y >> 5) + (Y >> 7);
            R = Y + (Cr << 1) + (Cr >> 6);
            if(R < 0) R = 0; else if(R > 255) R = 255;
            G = Y - Cb + (Cb >> 3) + (Cb >> 4) - (Cr >> 1) + (Cr >> 3);
            if(G < 0) G = 0; else if(G > 255) G = 255;
            B = Y + Cb + (Cb >> 1) + (Cb >> 4) + (Cb >> 5);
            if(B < 0) B = 0; else if(B > 255) B = 255;
            rgbData[pixPtr++] = 0xff000000 + (R << 16) + (G << 8) + B;
        }
    }

    env->ReleasePrimitiveArrayCritical(rgbOut, rgbData, 0);
    env->ReleasePrimitiveArrayCritical(yuv420sp, yuv, 0);
}

}

#endif