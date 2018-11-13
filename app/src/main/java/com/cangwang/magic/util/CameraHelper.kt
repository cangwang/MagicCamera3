package com.cangwang.magic.util

import android.app.Activity
import android.content.pm.PackageManager
import android.hardware.Camera
import android.util.Log
import android.view.Surface
import com.cangwang.magic.BaseApplication
import java.util.*


object CameraHelper{
    val TAG = CameraHelper::class.java.simpleName

    fun openCamera(): Camera? {
        return openCamera(Camera.CameraInfo.CAMERA_FACING_BACK)
    }

    fun openCamera(cameraId:Int):Camera?{
        if (!haveFeature(PackageManager.FEATURE_CAMERA)){
            Log.e(TAG,"no camera!")
            return null
        }

        if (cameraId == Camera.CameraInfo.CAMERA_FACING_FRONT && !haveFeature(PackageManager.FEATURE_CAMERA_FRONT)){
            Log.e(TAG,"no front camera!")
            return null
        }

        val camera = Camera.open(cameraId);
        if (camera == null){
            Log.e(TAG, "openCamera failed")
            return null
        }

        return camera
    }

    private fun haveFeature(name: String): Boolean {
        return BaseApplication.context?.packageManager?.hasSystemFeature(name)?:false
    }

    fun setOptimalSize(camera:Camera,aspectRatio:Float,maxWidth:Int,maxHeight:Int){
        val parameters= camera.parameters
        val size = chooseOptimalSize(parameters.supportedPreviewSizes,aspectRatio,maxWidth,maxHeight);
        parameters.setPreviewSize(size.width,size.height)
        Log.d(TAG, "input max: (" + maxWidth + ", " + maxHeight + "), output size: ("
                + size.width + ", " + size.height + ")")
        camera.parameters = parameters
    }

    fun chooseOptimalSize(options: List<Camera.Size>, aspectRatio: Float,
                          maxWidth: Int, maxHeight: Int): Camera.Size {
        val alternative = ArrayList<Camera.Size>()
        for (option in options) {
            if (option.height.toFloat() == option.width * aspectRatio && option.width <= maxWidth
                    && option.height <= maxHeight) {
                alternative.add(option)
            }
        }

        return if (alternative.size > 0) {
            Collections.max(alternative, CompareSizesByArea())
        } else options[0]

    }

    private class CompareSizesByArea : Comparator<Camera.Size> {

        override fun compare(lhs: Camera.Size, rhs: Camera.Size): Int {
            // 转型 long 是为了确保乘法运算不会溢出
            return java.lang.Long.signum(lhs.width.toLong() * lhs.height - rhs.width.toLong() * rhs.height)
        }

    }

    fun setDisplayOritation(activity: Activity, camera: Camera, cameraId: Int) {
        val rotation = activity.windowManager.defaultDisplay.rotation
        var degress = 0
        when (rotation) {
            Surface.ROTATION_0 -> degress = 0
            Surface.ROTATION_90 -> degress = 90
            Surface.ROTATION_180 -> degress = 180
            Surface.ROTATION_270 -> degress = 270
        }

        val info = Camera.CameraInfo()
        Camera.getCameraInfo(cameraId, info)
        var result: Int
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degress) % 360
            result = (360 - result) % 360  // compensate the mirror
        } else {
            result = (info.orientation - degress + 360) % 360 // back-facing
        }
        Log.d(TAG, "window rotation: $degress, camera oritation: $result")
        camera.setDisplayOrientation(result)
    }

    fun getScreenWidth(): Int {
        val metrics = BaseApplication.context?.resources?.displayMetrics
        return metrics?.widthPixels?:0
    }

    fun getScreenHeight(): Int {
        val metrics = BaseApplication.context?.resources?.displayMetrics
        return metrics?.heightPixels?:0
    }

    fun setFocusMode(camera: Camera, focusMode: String) {
        val parameters = camera.parameters
        val focusModes = parameters.supportedFocusModes
        if (focusModes.contains(focusMode)) {
            parameters.focusMode = focusMode
        }
        camera.parameters = parameters
    }
}