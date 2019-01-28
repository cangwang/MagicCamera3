package com.cangwang.magic.camera

import android.content.Context
import android.hardware.Camera
import android.hardware.Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO
import android.util.Log

class CameraCompatV19(context: Context) : CameraCompat(context) {

    private var mCamera: Camera? = null

    private var mIsLightOn: Boolean = false

    override fun initCameraInfo() {
        val numberOfCameras = Camera.getNumberOfCameras()
        val cameraInfo = Camera.CameraInfo()
        for (i in 0 until numberOfCameras) {
            Camera.getCameraInfo(i, cameraInfo)
            if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                setFrontCameraId(i)
            } else {
                setBackCameraId(i)
            }
        }
    }

    override fun onStartPreview() {
        try {
            mCamera?.setPreviewTexture(mSurfaceTexture)
            mCamera?.startPreview()
        } catch (e: Throwable) {
            Log.e(TAG, e.toString())
        }

    }

    override fun onStopPreview() {
        mCamera?.stopPreview()
        mCamera?.release()
        mCamera = null
        mCameraReady = false
    }

    override fun onOpenCamera(cameraType: Int) {
        try {
            mCamera = Camera.open(if (cameraType == FRONT_CAMERA)
                frontCameraIdV19
            else
                backCameraIdV19
            )
            initialize()
        } catch (e: Throwable) {
            Log.e(TAG, e.toString())
        }
    }

    override fun onTurnLight(on: Boolean) {
        if (mIsLightOn == on) {
            return
        }
        mIsLightOn = on
        val parameters = mCamera!!.parameters
        parameters.flashMode = if (on)
            Camera.Parameters.FLASH_MODE_TORCH
        else
            Camera.Parameters.FLASH_MODE_OFF
        mCamera?.parameters = parameters
    }

    private fun initialize() {
        if (mCamera == null) {
            return
        }
        try {
            mCamera?.setDisplayOrientation(90)
            val parameters = mCamera!!.parameters
            parameters.focusMode = FOCUS_MODE_CONTINUOUS_VIDEO
            val previewSizeList = parameters.supportedPreviewSizes
            outputSize = CameraUtil.findBestSize(DESIRED_HEIGHT, previewSizeList)
            outputSize?.let {
                parameters.setPreviewSize(it.width, it.height)
            }

            mCamera?.parameters = parameters
            if (mSurfaceTexture != null) {
                onStartPreview()
            }
            mCameraReady = true
        } catch (e: Throwable) {
            Log.e(TAG, "", e)
            mCameraReady = false
        }

    }

    companion object {

        private val TAG = "CameraCompatV19"
    }

}
