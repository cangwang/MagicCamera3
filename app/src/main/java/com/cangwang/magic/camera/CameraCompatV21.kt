package com.cangwang.magic.camera

import android.annotation.SuppressLint
import android.annotation.TargetApi
import android.content.Context
import android.graphics.Rect
import android.graphics.SurfaceTexture
import android.hardware.camera2.*
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import android.view.Surface
import java.lang.Exception


@TargetApi(21)
class CameraCompatV21(context: Context) : CameraCompat(context) {

    private var mManager: CameraManager? = null

    private var mCamera: CameraDevice? = null

    private var mCaptureSession: CameraCaptureSession? = null

    private var mIsFlashLightOn = false

    private var mRequestBuilder: CaptureRequest.Builder? = null

    private var mSurface: Surface? = null
    private var mBackgroundHandler: Handler? =null
    private var mBackgroundThread:HandlerThread? =null
    private var maxZoom = 0f

    /**
     * 放大的矩阵，拍照使用
     */
    private var mZoomRect: Rect? = null

    /**
     * 可缩放区域
     */
    private var mZoomSize: CameraCompat.CameraSize? = null

    private val mCaptureStateCallback = object : CameraCaptureSession.StateCallback() {
        override fun onConfigured(session: CameraCaptureSession) {
            if (mCamera == null) {
                return
            }
            mCaptureSession = session
            startRequest(session)
        }

        override fun onConfigureFailed(session: CameraCaptureSession) {
            Log.e(TAG, "onConfigureFailed")
        }
    }

    private val mStateCallback = object : CameraDevice.StateCallback() {
        override fun onOpened(camera: CameraDevice) {
            mCamera = camera
            mCameraReady = true
            mStarted = false
            if (mSurface != null) {
                startPreview()
            }
            Log.d(TAG, "onOpened")
        }

        override fun onDisconnected(camera: CameraDevice) {
            mCamera = null
            Log.d(TAG, "onDisconnected")
            mCameraReady = false
        }

        override fun onError(camera: CameraDevice, error: Int) {
            mCamera = null
            mCameraReady = false
            Log.d(TAG, "open camera onError $error")
        }
    }

    override fun initCameraInfo() {
        //返回当前可用的相机列表
        mManager = mContext.getSystemService(Context.CAMERA_SERVICE) as CameraManager
        if (mManager == null) {
            return
        }
        try {
            //获取可用摄像头列表
            for (cameraId in mManager!!.cameraIdList) {
                //获取相机的相关参数
                val characteristics = mManager!!.getCameraCharacteristics(cameraId)
                val facing = characteristics.get(CameraCharacteristics.LENS_FACING) ?: continue
                val map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
                        ?: continue
                if (facing == CameraCharacteristics.LENS_FACING_BACK) {
                    setBackCameraId(cameraId) //记录backCameraId
                } else {
                    setFrontCameraId(cameraId)  //记录frontCameraId
                }
            }
        } catch (e: Throwable) {
            Log.e(TAG, "", e)
        }

    }

    private fun updateOutputSize() {
        try {
            val characteristics = mManager!!.getCameraCharacteristics(if (mCameraType == FRONT_CAMERA)
                frontCameraIdV21
            else
                backCameraIdV21)
            val map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
                    ?: return
            //获取支持尺寸
            val sizes = map.getOutputSizes(SurfaceTexture::class.java)
            outputSize = CameraUtil.getLargePreviewSize(sizes)
            mZoomSize = outputSize
        } catch (e: CameraAccessException) {
            Log.e(TAG, e.toString())
        }

    }


    private fun startRequest(session: CameraCaptureSession?) {
        try {
            session?.setRepeatingRequest(mRequestBuilder?.build(), null, mBackgroundHandler)
        } catch (e: Throwable) {
            Log.e(TAG, "", e)
        }

    }

    private fun abortSession() {
        try {
            mCaptureSession?.abortCaptures()
        } catch (e: Throwable) {
            Log.e(TAG, "", e)
        }

    }


    override fun onStartPreview(callBack: CameraStateCallBack?) {
        try {
            mCaptureSession?.close()

            mSurface = Surface(mSurfaceTexture)

            outputSize?.let {
                mSurfaceTexture?.setDefaultBufferSize(it.width, it.height)
            }
            //创建捕抓会话 TEMPLATE_RECORD 创建适合录像的请求
            mRequestBuilder = mCamera?.createCaptureRequest(CameraDevice.TEMPLATE_RECORD)
            mSurface?.let {
                mRequestBuilder?.addTarget(it)
                if (mZoomRect != null)
                    mRequestBuilder?.set(CaptureRequest.SCALER_CROP_REGION, mZoomRect)   //放大的矩阵
                mRequestBuilder?.set(CaptureRequest.CONTROL_AF_MODE,
                        CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_VIDEO)
                mRequestBuilder?.set(CaptureRequest.FLASH_MODE, if (mIsFlashLightOn)
                    CaptureRequest.FLASH_MODE_TORCH
                else
                    CaptureRequest.FLASH_MODE_OFF)

                mCamera?.createCaptureSession(listOf(it),
                        object : CameraCaptureSession.StateCallback() {
                            override fun onConfigured(session: CameraCaptureSession) {
                                if (mCamera == null) {
                                    return
                                }
                                mCaptureSession = session
                                startRequest(session)
                                callBack?.onConfigured()
                            }

                            override fun onConfigureFailed(session: CameraCaptureSession) {
                                Log.e(TAG, "onConfigureFailed")
                                callBack?.onConfigureFailed()
                            }
                        }, mBackgroundHandler)
            }

        } catch (e: Throwable) {
            Log.e(TAG, "", e)
        }

    }

    override fun onStopPreview() {
        mSurface?.release()
        abortSession()
        mCamera?.close()
        mCamera = null
        stopBackgroundThread()
    }

    override fun onOpenCamera(@CameraType cameraType: Int) {
        initialize(cameraType)
    }

    /**
     * 开启HandlerThread
     */
    fun startBackgroundThread(){
        mBackgroundThread = HandlerThread("CameraBackground")
        mBackgroundThread?.start()
        mBackgroundHandler = Handler(mBackgroundThread?.looper)
    }

    fun stopBackgroundThread(){
        try {
            mBackgroundThread?.apply {
                quitSafely()
                join()
            }
            mBackgroundThread = null
            mBackgroundHandler =null
        }catch (e:InterruptedException){
            Log.e(TAG,e.toString())
        }
    }

    override fun getMaxZoom():Float{
        return maxZoom
    }

    override fun cameraZoom(sc:Float){
        val scale = if(sc < 1.0f) 1.0f
                    else sc
        if (scale <= maxZoom) {
            mZoomSize?.apply {
                val cropW = (width / (maxZoom * 2.6) * scale).toInt()
                val cropH = (height / (maxZoom * 2.6) * scale).toInt()

                val zoom = Rect(cropW, cropH,
                        width - cropW,
                        height - cropH)
                mRequestBuilder?.set(CaptureRequest.SCALER_CROP_REGION, zoom)
                mZoomRect = zoom
                updatePreview()   //重复更新预览请求
            }
        }
    }

    fun updatePreview(){
        if (mCamera!=null){
            try {
                mCaptureSession?.setRepeatingRequest(mRequestBuilder?.build(),null,mBackgroundHandler)
            }catch (e:Exception){
                Log.e(TAG,e.toString())
            }
        }
    }

    @SuppressLint("MissingPermission")
    private fun initialize(@CameraType cameraType: Int) {
        try {
            startBackgroundThread()
            val id = if (cameraType == FRONT_CAMERA)
                frontCameraIdV21
            else
                backCameraIdV21
            //打开摄像头
            mManager?.openCamera(id, mStateCallback, mBackgroundHandler)
            updateOutputSize()
            val mCharacteristics = mManager?.getCameraCharacteristics(id)
            maxZoom = mCharacteristics?.get(CameraCharacteristics.SCALER_AVAILABLE_MAX_DIGITAL_ZOOM)?:0f
        } catch (e: Throwable) {
            Log.e(TAG, "", e)
        }

    }

    override fun onTurnLight(on: Boolean) {
        if (mIsFlashLightOn == on) {
            return
        }
        mIsFlashLightOn = on
        if (mCameraType == FRONT_CAMERA && on) {
            return
        }
        abortSession()
        mRequestBuilder?.set(CaptureRequest.FLASH_MODE, if (mIsFlashLightOn)
            CaptureRequest.FLASH_MODE_TORCH
        else
            CaptureRequest.FLASH_MODE_OFF)
        startRequest(mCaptureSession)
    }

    override fun requestFocus(x: Int, y: Int) {

    }

    fun calcTapAreaForCamera2(areaSize:Int,widght:Int,x:Float,y:Float){

    }

    fun clamp(modes:IntArray,mode:Int):Boolean{
        modes.forEach { if (it == mode) return true }
        return false
    }

    companion object {

        private val TAG = "CameraCompatV21"
    }
}
