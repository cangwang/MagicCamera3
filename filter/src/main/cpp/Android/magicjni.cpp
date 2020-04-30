#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <mutex>
#include <thread>
#include <cstdio>
#include <malloc.h>
#include <GLES3/gl3.h>
#include <filter/MagicFilterFactory.h>
#include <image/ImageFilter.h>
#include <video/VideoFilter.h>
#include "camera/CameraEngine.h"
#include "camera/CameraFilter.h"

#include "filter/source/SourceImage.h"
#include "filter/source/SourceCamera.h"
#include "filter/source/TargetView.h"
#include "filter/effect/Filter.hpp"
#include "filter/source/Context.hpp"

#define LOG_TAG "magicjni"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


std::mutex gMutex;
CameraEngine *glCamera = nullptr;
CameraFilter *glCameraFilter = nullptr;
VideoFilter *glVideoFilter = nullptr;
ImageFilter *glImageFilter = nullptr;
AAssetManager *aAssetManager = nullptr;

static const char* const kClassDocScanner = "com/cangwang/filter/util/OpenGLJniLib";


static jint magicBaseInit(JNIEnv *env, jclass type,
        jobject surface,jint width,jint height,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCamera){
        glCamera->stop();
        delete glCamera;
    }
    //创建window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //获取文件管理
    AAssetManager *manager = AAssetManager_fromJava(env,assetManager);
    glCamera = new CameraEngine(window);
    glCamera->setAssetManager(manager);
    glCamera->resize(width,height);
    return glCamera->create();
}


static void magicBaseCreate(JNIEnv *env, jobject obj) {
    //关闭颜色抖动 若机器的分辨率已经相当高，激活抖动操作根本就没有任何意义
    glDisable(GL_DITHER);
    //清颜色
    glClearColor(0.0,0.0,0.0,0.0);
    //开启剔除操作效果 在多边形中使用
    glEnable(GL_CULL_FACE);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
}

static void magicBaseChange(JNIEnv *env, jclass type, jint width, jint height) {
    //视口变换，可视区域
    if(glCamera)
        glCamera->resize(width,height);
}

static void magicBaseDraw(JNIEnv *env, jclass type, jfloatArray matrix_) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);

    std::unique_lock<std::mutex> lock(gMutex);
    if (!glCamera){
        ALOGE("draw error, glCamera is null");
        return;
    }
    glCamera->draw(matrix);
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

static void magicBaseRelease(JNIEnv *env, jobject obj) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glCamera){
        glCamera->stop();
        delete glCamera;
        glCamera = nullptr;
    }
}

static void magicAdjustSize(JNIEnv *env, jclass type, jint orientation, jboolean isFront,
        jboolean flipVertical) {

}

//相机滤镜surfaceView初始化的时候创建
static jint magicFilterCreate(JNIEnv *env, jclass type, jobject surface,jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glCameraFilter){ //停止摄像头采集并销毁
        glCameraFilter->stop();
        delete glCameraFilter;
    }

//初始化native window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
//初始化app内获取数据管理
    aAssetManager= AAssetManager_fromJava(env,assetManager);
//初始化相机采集
    glCameraFilter = new CameraFilter(window,aAssetManager);
//创建
    return glCameraFilter->create();
}

//static void magicFilterSet(JNIEnv *env, jclass type,
//                           jobject surface,jobject assetManager) {
//    std::unique_lock<std::mutex> lock(gMutex);
//    if (!glCameraFilter){
//        ALOGE("filter set error, glCameraFilter is null");
//        return;
//    }
//    AAssetManager *manager = AAssetManager_fromJava(env,assetManager);
//    glCameraFilter->setFilter(manager);
//    if(glVideoFilter!= nullptr){
//        glVideoFilter->setFilter(manager);
//    }
//}

//窗口大小设置，SurfaceView初始化后会触发一次
static void magicFilterChange(JNIEnv *env, jclass type, jint width, jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glCameraFilter){
        ALOGE("change error, glCameraFilter is null");
        return;
    }
    //更改窗口大小
    glCameraFilter->change(width,height);
    if(glVideoFilter!= nullptr){
        glVideoFilter->change(width,height);
    }
}

static void magicFilterDraw(JNIEnv *env, jclass type, jfloatArray matrix_, jstring address) {
    //获取摄像头矩阵
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);
    //加锁
    std::unique_lock<std::mutex> lock(gMutex);
    //如果为空，就判断错误，中断
    if (!glCameraFilter){
        ALOGE("draw error, glCameraFilter is null");
        return;
    }
    //摄像头采集画图
    glCameraFilter->draw(matrix);
    //释放矩阵数据
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

static void magicFilterRelease(JNIEnv *env, jclass type) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glCameraFilter){
        glCameraFilter->stop();
        delete glCameraFilter;
        glCameraFilter = nullptr;
    }
}

static jint buildVideoSurface(JNIEnv *env, jclass type, jobject surface, jint textureId,
        jobject assetManager) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glVideoFilter){ //停止视频采集并销毁
        glVideoFilter->stop();
        delete glVideoFilter;
        glVideoFilter = nullptr;
    }

    //初始化native window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //初始化app内获取数据管理
    aAssetManager= AAssetManager_fromJava(env,assetManager);
    //初始化相机采集
    glVideoFilter = new VideoFilter(window,aAssetManager);
    //创建
    if(glCameraFilter!= nullptr)
        return glVideoFilter->create(textureId,glCameraFilter->getCurrentContext());
    return -1;
}

//窗口大小设置，SurfaceView初始化后会触发一次
static void magicVideoFilterChange(JNIEnv *env, jclass type, jint width, jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glVideoFilter){
        ALOGE("change error, glCameraFilter is null");
        return;
    }
    //更改窗口大小
    glVideoFilter->change(width,height);
}

static void magicVideoDraw(JNIEnv *env, jclass type, jfloatArray matrix_, jlong time) {
    //加锁
    std::unique_lock<std::mutex> lock(gMutex);
    //获取摄像头矩阵
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);

    //如果为空，就判断错误，中断
    if (!glVideoFilter){
        ALOGE("draw error, glVideoFilter is null");
        return;
    }
    //摄像头采集画图
    glVideoFilter->draw(matrix, static_cast<uint64_t>(time));
    //释放矩阵数据
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

static void releaseVideoSurface(JNIEnv *env, jclass type) {
    std::unique_lock<std::mutex> lock(gMutex);

    if(glVideoFilter){ //停止摄像头采集并销毁
        glVideoFilter->stop();
        delete glVideoFilter;
        glVideoFilter = nullptr;
    }
}

//图片滤镜surfaceView初始化的时候创建
static jint magicImageFilterCreate(JNIEnv *env, jclass type,
        jobject surface,jobject assetManager,jstring imgPath,jint degree) {
    std::unique_lock<std::mutex> lock(gMutex);
    if(glImageFilter){ //停止摄像头采集并销毁
        glImageFilter->stop();
        delete glImageFilter;
        glImageFilter = nullptr;
    }

    //初始化native window
    ANativeWindow *window = ANativeWindow_fromSurface(env,surface);
    //初始化app内获取数据管理
    aAssetManager= AAssetManager_fromJava(env,assetManager);
    //初始化图片
    const char* addressStr = env->GetStringUTFChars(imgPath,0);
    std::string nativeAddress = addressStr;

    glImageFilter = new ImageFilter(window,aAssetManager,nativeAddress,degree);
    env->ReleaseStringUTFChars(imgPath, addressStr);
    //创建
    return glImageFilter->create();
}

//窗口大小设置，SurfaceView初始化后会触发一次
static void magicImageFilterChange(JNIEnv *env, jclass type, jint width, jint height) {
    std::unique_lock<std::mutex> lock(gMutex);
    //视口变换，可视区域
    if (!glImageFilter){
        ALOGE("change error, glImageFilter is null");
        return;
    }
    //更改窗口大小
    glImageFilter->change(width,height);
}

static void magicImageFilterDraw(JNIEnv *env, jclass type, jfloatArray matrix_,
        jstring address) {
    //获取摄像头矩阵
    jfloat *matrix = env->GetFloatArrayElements(matrix_,NULL);
    //加锁
    std::unique_lock<std::mutex> lock(gMutex);
    //如果为空，就判断错误，中断
    if (!glImageFilter){
        ALOGE("draw error, glImageFilter is null");
        return;
    }
    //图片和滤镜绘制
    glImageFilter->draw(matrix);
    //释放矩阵数据
    env->ReleaseFloatArrayElements(matrix_,matrix,0);
}

static void magicImageFilterRelease(JNIEnv *env, jclass type) {
    std::unique_lock<std::mutex> lock(gMutex);
    if (glImageFilter){
        glImageFilter->stop();
        delete glImageFilter;
        glImageFilter = nullptr;
    }
}

static jintArray getFilterType(JNIEnv *env, jclass type) {
    int len =0;
    jint* types = getFilterTypes(len);
    jintArray jin_arr=(env)->NewIntArray(len);
    (env)->SetIntArrayRegion(jin_arr,0,len,types);
    return jin_arr;
}

static void setFilterType(JNIEnv *env, jclass typ, jint type) {
    if(glCameraFilter!= nullptr)
        glCameraFilter->setFilter(type);
    else ALOGE("filter set error, glCameraFilter is null");
    if(glVideoFilter!= nullptr)
        glVideoFilter->setFilter(type);
}

static void setVideoFilterType(JNIEnv *env, jclass typ, jint type) {
    if(glVideoFilter!= nullptr)
        glVideoFilter->setFilter(type);
    else ALOGE("filter set error, glVideoFilter is null");
}

static void setImageFilterType(JNIEnv *env, jclass typ, jint type) {
    if(glImageFilter!= nullptr)
        glImageFilter->setFilter(type);
    else ALOGE("filter set error, glImageFilter is null");
}

static void setImageBeautyLevel(JNIEnv *env, jclass typ, jint level) {
    if(glImageFilter!= nullptr)
        glImageFilter->setBeautyLevel(level);
    else ALOGE("beauty set error, glCameraFilter is null");
}


static void setBeautyLevel(JNIEnv *env, jclass type, jint level) {
    if(glCameraFilter!= nullptr)
        glCameraFilter->setBeautyLevel(level);
    else ALOGE("beauty set error, glCameraFilter is null");
}

static jboolean savePhoto(JNIEnv *env, jclass typ, jstring address) {
    if (glCameraFilter != nullptr) {
        const char* addressStr = env->GetStringUTFChars(address,0);
        std::string nativeAddress = addressStr;
        bool result = false;
        if(glCameraFilter!= nullptr)
            result = glCameraFilter->savePhoto(nativeAddress);
        env->ReleaseStringUTFChars(address, addressStr);
        return static_cast<jboolean>(result);
    } else{
        ALOGE("save photo error, glCameraFilter is null");
        return static_cast<jboolean>(false);
    }
}

static jboolean saveImage(JNIEnv *env, jclass typ, jstring address) {
    if (glImageFilter != nullptr) {
        const char* addressStr = env->GetStringUTFChars(address,0);

        std::string nativeAddress = addressStr;
        bool result = false;
        if(glImageFilter!= nullptr)
            result = glImageFilter->saveImage(nativeAddress);
        env->ReleaseStringUTFChars(address, addressStr);
        return static_cast<jboolean>(result);
    }else{
        ALOGE("save photo error, glImageFilter is null");
        return static_cast<jboolean>(false);
    }
}


extern "C" {
static JNINativeMethod gMethods[] = {
        {
                "magicBaseInit",
                "(Landroid/view/Surface;IILandroid/content/res/AssetManager;)I",
                (void*)magicBaseInit
        },
        {
                "magicBaseCreate",
                "()V",
                (void*)magicBaseCreate
        },
        {
                "magicBaseChange",
                "(II)V",
                (void*)magicBaseChange
        },
        {
                "magicBaseDraw",
                "([F)V",
                (void*)magicBaseDraw
        },
        {
                "magicBaseRelease",
                "()V",
                (void*)magicBaseRelease
        },
        {
                "magicAdjustSize",
                "(IZZ)V",
                (void*)magicAdjustSize
        },
        {
                "magicFilterCreate",
                "(Landroid/view/Surface;Landroid/content/res/AssetManager;)I",
                (void*)magicFilterCreate
        },
//        {
//                "magicFilterSet",
//                "(Landroid/view/Surface;Landroid/content/res/AssetManager;)V",
//                (void*)magicFilterSet
//        },
        {
                "magicFilterChange",
                "(II)V",
                (void*)magicFilterChange
        },
        {
                "magicFilterDraw",
                "([FLjava/lang/String;)V",
                (void*)magicFilterDraw
        },
        {
                "magicFilterRelease",
                "()V",
                (void*)magicFilterRelease
        },
        {
                "buildVideoSurface",
                "(Landroid/view/Surface;ILandroid/content/res/AssetManager;)I",
                (void*)buildVideoSurface
        },
        {
                "magicVideoFilterChange",
                "(II)V",
                (void*)magicVideoFilterChange
        },
        {
                "magicVideoDraw",
                "([FJ)V",
                (void*)magicVideoDraw
        },
        {
                "releaseVideoSurface",
                "()V",
                (void*)releaseVideoSurface
        },
        {
                "magicImageFilterCreate",
                "(Landroid/view/Surface;Landroid/content/res/AssetManager;Ljava/lang/String;I)I",
                (void*)magicImageFilterCreate
        },
        {
                "magicImageFilterChange",
                "(II)V",
                (void*)magicImageFilterChange
        },
        {
                "magicImageFilterDraw",
                "([FLjava/lang/String;)V",
                (void*)magicImageFilterDraw
        },
        {
                "magicImageFilterRelease",
                "()V",
                (void*)magicImageFilterRelease
        },
        {
                "getFilterType",
                "()[I",
                (void*)getFilterType
        },
        {
                "setFilterType",
                "(I)V",
                (void*)setFilterType
        },
        {
                "setVideoFilterType",
                "(I)V",
                (void*)setVideoFilterType
        },
        {
                "setImageFilterType",
                "(I)V",
                (void*)setImageFilterType
        },
        {
                "setImageBeautyLevel",
                "(I)V",
                (void*)setImageBeautyLevel
        },
        {
                "setBeautyLevel",
                "(I)V",
                (void*)setBeautyLevel
        },
        {
                "savePhoto",
                "(Ljava/lang/String;)Z",
                (void*)savePhoto
        },
        {
                "saveImage",
                "(Ljava/lang/String;)Z",
                (void*)saveImage
        }
};

USING_NS_GI
extern "C" {
jlong Java_com_jin_gpuimage_GPUImage_nativeSourceImageNew(
        JNIEnv *env,
        jobject) {
    return (uintptr_t) (new SourceImage());
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceImageDestroy(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((SourceImage *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceImageFinalize(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((SourceImage *) classId)->releaseFramebuffer(false);
    ((SourceImage *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceImageSetImage(
        JNIEnv *env,
        jobject,
        jlong classId,
        jobject bitmap) {
    char *pData = 0;
    AndroidBitmapInfo info;
    void *pixels;
    if ((AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        //ERROR
        return;
    }

    if ((AndroidBitmap_lockPixels(env, bitmap, &pixels)) >= 0) {
        ((SourceImage *) classId)->setImage(info.width, info.height, pixels);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
} ;

jlong Java_com_jin_gpuimage_GPUImage_nativeSourceCameraNew(
        JNIEnv *env,
        jobject) {
    return (uintptr_t) (new SourceCamera());
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceCameraDestroy(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((SourceCamera *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceCameraFinalize(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((SourceCamera *) classId)->releaseFramebuffer(false);
    ((SourceCamera *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceCameraSetFrame(
        JNIEnv *env,
        jobject,
        jlong classId,
        jint width,
        jint height,
        jintArray jdata,
        jint rotation) {
    jint *data = (jint *) (env->GetPrimitiveArrayCritical(jdata, 0));
    ((SourceCamera *) classId)->setFrameData(width, height, data, (RotationMode) rotation);
    env->ReleasePrimitiveArrayCritical(jdata, data, 0);
} ;

jlong Java_com_jin_gpuimage_GPUImage_nativeSourceAddTarget(
        JNIEnv *env,
        jobject,
        jlong classId,
        jlong targetClassId,
        jint texID,
        jboolean isFilter) {
    Source *source = (Source *) classId;
    Target *target = isFilter ? dynamic_cast<Target *>((Filter *) targetClassId)
                              : (Target *) targetClassId;
    if (texID >= 0) {
        return (uintptr_t) (source->addTarget(target, texID));
    } else {
        return (uintptr_t) (source->addTarget(target));
    }
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceRemoveTarget(
        JNIEnv *env,
        jobject,
        jlong classId,
        jlong targetClassId,
        jboolean isFilter) {
    Source *source = (Source *) classId;
    Target *target = isFilter ? dynamic_cast<Target *>((Filter *) targetClassId)
                              : (Target *) targetClassId;
    source->removeTarget(target);
} ;

void Java_com_jin_gpuimage_GPUImage_nativeSourceRemoveAllTargets(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((Source *) classId)->removeAllTargets();
} ;

jlong Java_com_jin_gpuimage_GPUImage_nativeSourceProceed(
        JNIEnv *env,
        jobject,
        jlong classId,
        jboolean bUpdateTargets) {
    return ((Source *) classId)->proceed(bUpdateTargets);
} ;

jint Java_com_jin_gpuimage_GPUImage_nativeSourceGetRotatedFramebuferWidth(
        JNIEnv *env,
        jobject,
        jlong classId) {
    return ((Source *) classId)->getRotatedFramebufferWidth();
} ;

jint Java_com_jin_gpuimage_GPUImage_nativeSourceGetRotatedFramebuferHeight(
        JNIEnv *env,
        jobject,
        jlong classId) {
    return ((Source *) classId)->getRotatedFramebufferHeight();
} ;

jbyteArray Java_com_jin_gpuimage_GPUImage_nativeSourceCaptureAProcessedFrameData(
        JNIEnv *env,
        jobject,
        jlong classId,
        jlong upToFilterClassId,
        jint width,
        jint height) {
    unsigned char *processedFrameData = ((Source *) classId)->captureAProcessedFrameData(
            (Filter *) upToFilterClassId, width, height);
    int frameSize = width * height * 4 * sizeof(unsigned char);

    jbyteArray jresult = NULL;
    if (processedFrameData) {
        jbyte *by = (jbyte *) processedFrameData;
        jresult = env->NewByteArray(frameSize);
        env->SetByteArrayRegion(jresult, 0, frameSize, by);
        delete[] processedFrameData;
        processedFrameData = 0;
    }

    return jresult;
} ;

jlong Java_com_jin_gpuimage_GPUImage_nativeTargetViewNew(
        JNIEnv *env,
        jobject obj) {
    return (uintptr_t) (new TargetView());
} ;

void Java_com_jin_gpuimage_GPUImage_nativeTargetViewFinalize(
        JNIEnv *env,
        jobject,
        jlong classId) {
    ((TargetView *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeTargetViewOnSizeChanged(
        JNIEnv *env,
        jobject,
        jlong classId,
        jint width,
        jint height) {
    ((TargetView *) classId)->onSizeChanged(width, height);
} ;

void Java_com_jin_gpuimage_GPUImage_nativeTargetViewSetFillMode(
        JNIEnv *env,
        jobject,
        jlong classId,
        jint fillMode) {
    ((TargetView *) classId)->setFillMode((TargetView::FillMode) fillMode);
} ;

jlong Java_com_jin_gpuimage_GPUImage_nativeFilterCreate(
        JNIEnv *env,
        jobject obj,
        jstring jFilterClassName) {
    const char *filterClassName = env->GetStringUTFChars(jFilterClassName, 0);
    long ret = (uintptr_t) (Filter::create(filterClassName));
    env->ReleaseStringUTFChars(jFilterClassName, filterClassName);
    return ret;
} ;

void Java_com_jin_gpuimage_GPUImage_nativeFilterDestroy(
        JNIEnv *env,
        jobject obj,
        jlong classId) {
    ((Filter *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeFilterFinalize(
        JNIEnv *env,
        jobject obj,
        jlong classId) {
    ((Filter *) classId)->releaseFramebuffer(false);
    ((Filter *) classId)->release();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeFilterSetPropertyFloat(
        JNIEnv *env,
        jobject obj,
        jlong classId,
        jstring jProperty,
        jfloat value) {
    const char *property = env->GetStringUTFChars(jProperty, 0);
    ((Filter *) classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);

} ;

void Java_com_jin_gpuimage_GPUImage_nativeFilterSetPropertyInt(
        JNIEnv *env,
        jobject obj,
        jlong classId,
        jstring jProperty,
        jint value) {
    const char *property = env->GetStringUTFChars(jProperty, 0);
    ((Filter *) classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);

} ;

void Java_com_jin_gpuimage_GPUImage_nativeFilterSetPropertyString(
        JNIEnv *env,
        jobject obj,
        jlong classId,
        jstring jProperty,
        jstring jValue) {
    const char *property = env->GetStringUTFChars(jProperty, 0);
    const char *value = env->GetStringUTFChars(jValue, 0);
    ((Filter *) classId)->setProperty(property, value);
    env->ReleaseStringUTFChars(jProperty, property);
    env->ReleaseStringUTFChars(jValue, value);

} ;

void Java_com_jin_gpuimage_GPUImage_nativeContextInit(
        JNIEnv *env,
        jobject obj) {
    Context::init();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeContextDestroy(
        JNIEnv *env,
        jobject obj) {
    Context::destroy();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeContextPurge(
        JNIEnv *env,
        jobject obj) {
    Context::getInstance()->purge();
} ;

void Java_com_jin_gpuimage_GPUImage_nativeYUVtoRBGA(JNIEnv *env, jobject obj, jbyteArray yuv420sp,
                                                    jint width, jint height, jintArray rgbOut) {
    int sz;
    int i;
    int j;
    int Y;
    int Cr = 0;
    int Cb = 0;
    int pixPtr = 0;
    int jDiv2 = 0;
    int R = 0;
    int G = 0;
    int B = 0;
    int cOff;
    int w = width;
    int h = height;
    sz = w * h;

    jint *rgbData = (jint *) (env->GetPrimitiveArrayCritical(rgbOut, 0));
    jbyte *yuv = (jbyte *) env->GetPrimitiveArrayCritical(yuv420sp, 0);

    for (j = 0; j < h; j++) {
        pixPtr = j * w;
        jDiv2 = j >> 1;
        for (i = 0; i < w; i++) {
            Y = yuv[pixPtr];
            if (Y < 0) Y += 255;
            if ((i & 0x1) != 1) {
                cOff = sz + jDiv2 * w + (i >> 1) * 2;
                Cb = yuv[cOff];
                if (Cb < 0) Cb += 127; else Cb -= 128;
                Cr = yuv[cOff + 1];
                if (Cr < 0) Cr += 127; else Cr -= 128;
            }

            //ITU-R BT.601 conversion
            //
            //R = 1.164*(Y-16) + 2.018*(Cr-128);
            //G = 1.164*(Y-16) - 0.813*(Cb-128) - 0.391*(Cr-128);
            //B = 1.164*(Y-16) + 1.596*(Cb-128);
            //
            Y = Y + (Y >> 3) + (Y >> 5) + (Y >> 7);
            R = Y + (Cr << 1) + (Cr >> 6);
            if (R < 0) R = 0; else if (R > 255) R = 255;
            G = Y - Cb + (Cb >> 3) + (Cb >> 4) - (Cr >> 1) + (Cr >> 3);
            if (G < 0) G = 0; else if (G > 255) G = 255;
            B = Y + Cb + (Cb >> 1) + (Cb >> 4) + (Cb >> 5);
            if (B < 0) B = 0; else if (B > 255) B = 255;
            rgbData[pixPtr++] = 0xff000000 + (R << 16) + (G << 8) + B;
        }
    }

    env->ReleasePrimitiveArrayCritical(rgbOut, rgbData, 0);
    env->ReleasePrimitiveArrayCritical(yuv420sp, yuv, 0);
}

}


JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv *env = NULL;
    if(vm->GetEnv((void **) &env,JNI_VERSION_1_4)!=JNI_OK){
        return JNI_FALSE;
    }
    jclass classDocScanner = env->FindClass(kClassDocScanner);
    if(env->RegisterNatives(classDocScanner,gMethods, sizeof(gMethods)/ sizeof(gMethods[0])) < 0){
        return JNI_FALSE;
    }

    return JNI_VERSION_1_4;
}

}
