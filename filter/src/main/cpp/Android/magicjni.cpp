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
