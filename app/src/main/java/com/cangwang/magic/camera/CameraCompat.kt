package com.cangwang.magic.camera

import android.annotation.TargetApi
import android.content.Context
import android.content.pm.PackageManager
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.os.Build
import android.support.annotation.IntDef
import android.util.Size
import android.util.SparseArray
import java.lang.annotation.Retention
import java.lang.annotation.RetentionPolicy

abstract class CameraCompat(protected var mContext: Context) {

    private var mSwitchFlag: Boolean = false

    protected var mSurfaceTexture: SurfaceTexture? = null
    /**
     * 是否支持闪光灯
     */
    private var mSupportFlash = false
    /**
     * 相机是否初始化完成
     */
    protected var mCameraReady: Boolean = false
    /**
     * 是否开始预览
     */
    protected var mStarted: Boolean = false

    @CameraType
    protected var mCameraType = BACK_CAMERA

    var outputSize: CameraSize? = null

    private val mCameraInfo = SparseArray<String>(2)

    protected val frontCameraIdV19: Int
        get() = Integer.valueOf(mCameraInfo.get(FRONT_CAMERA))

    protected val backCameraIdV19: Int
        get() = Integer.valueOf(mCameraInfo.get(BACK_CAMERA))

    protected val frontCameraIdV21: String
        get() = mCameraInfo.get(FRONT_CAMERA)

    protected val backCameraIdV21: String
        get() = mCameraInfo.get(BACK_CAMERA)

    @IntDef(FRONT_CAMERA, BACK_CAMERA)
    @Retention(RetentionPolicy.SOURCE)
    annotation class CameraType

    init {
        mSupportFlash = mContext.packageManager.hasSystemFeature(PackageManager.FEATURE_CAMERA_FLASH)
        initCameraInfo()
    }

    fun setSurfaceTexture(texture: SurfaceTexture) {
        this.mSurfaceTexture = texture
    }

    protected fun setFrontCameraId(id: String) {
        mCameraInfo.put(FRONT_CAMERA, id)
    }

    protected fun setFrontCameraId(id: Int) {
        mCameraInfo.put(FRONT_CAMERA, id.toString())
    }

    protected fun setBackCameraId(id: Int) {
        mCameraInfo.put(BACK_CAMERA, id.toString())
    }

    protected fun setBackCameraId(id: String) {
        mCameraInfo.put(BACK_CAMERA, id)
    }

    protected abstract fun initCameraInfo()

    /**
     * 这里会两次调用此函数，第一次会初始化走openCamera，
     * 第二次调用才能正常开始预览
     */
    fun startPreview(callBack: CameraStateCallBack?=null) {
//        if (Looper.myLooper() != Looper.getMainLooper()) {
//            throw RuntimeException("you must start camera preview in main thread")
//        }
        if (mStarted) {
            return
        }
        if (mSwitchFlag) {
            mStarted = true
        }
        if (!mCameraReady) {
            openCamera(mCameraType)
            return
        }
        if (mSurfaceTexture == null) {
            return
        }
        mStarted = true
        onStartPreview(callBack)
    }

    protected abstract fun onStartPreview(callBack: CameraStateCallBack?=null)

    fun stopPreview(releaseSurface: Boolean) {
        if (!mStarted) {
            return
        }
        mCameraReady = false
        mStarted = false
        if (releaseSurface) {
            mSurfaceTexture = null
        }
        onStopPreview()
    }

    abstract fun onStopPreview()

    protected fun openCamera(@CameraType cameraType: Int) {
        mCameraType = cameraType
        onOpenCamera(cameraType)
    }

    protected abstract fun onOpenCamera(@CameraType cameraType: Int)


    fun turnLight(on: Boolean) {
        if (!mSupportFlash) {
            return
        }
        onTurnLight(on)
    }

    protected abstract fun onTurnLight(on: Boolean)


    fun switchCamera() {
        if (!mStarted) {
            return
        }
        mSwitchFlag = true
        turnLight(false)
        stopPreview(false)
        mCameraType = if (mCameraType == FRONT_CAMERA) BACK_CAMERA else FRONT_CAMERA
        startPreview()
        mSwitchFlag = false
    }

    fun getCameraType():Int{
        return if (mCameraType == FRONT_CAMERA) BACK_CAMERA else FRONT_CAMERA
    }

    open fun getMaxZoom():Float{
        return 0f
    }

    open fun cameraZoom(scale:Float){

    }

    class CameraSize {
        var width: Int = 0
        var height: Int = 0

        constructor(size: Camera.Size) {
            width = size.width
            height = size.height
        }

        @TargetApi(21)
        constructor(size: Size) {
            width = size.width
            height = size.height
        }
    }

    companion object {

        const val DESIRED_HEIGHT = 720

        const val FRONT_CAMERA = 1

        const val BACK_CAMERA = 2

        fun newInstance(context: Context): CameraCompat {
            val api = Build.VERSION.SDK_INT
            return if (api >= 21) {
                CameraCompatV21(context)
            } else {
                CameraCompatV19(context)
            }
//                    return CameraCompatV19(context)
        }
    }

    interface CameraStateCallBack{
        fun onConfigured()
        fun onConfigureFailed()
    }
}
