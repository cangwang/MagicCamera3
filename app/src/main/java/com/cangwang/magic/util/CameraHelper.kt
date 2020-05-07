package com.cangwang.magic.util

import android.app.Activity
import android.content.pm.PackageManager
import android.hardware.Camera
import android.util.Log
import android.view.Surface
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.camera.CameraInfo
import java.util.*

object CameraHelper {
    val TAG = CameraHelper::class.java.simpleName
    val cameraId = Camera.CameraInfo.CAMERA_FACING_BACK

    fun openCamera(): Camera? {
        return openCamera(cameraId)
    }

    fun openCamera(cameraId: Int): Camera? {
        if (!haveFeature(PackageManager.FEATURE_CAMERA)) {
            Log.e(TAG, "no camera!")
            return null
        }

        if (cameraId == Camera.CameraInfo.CAMERA_FACING_FRONT && !haveFeature(
                        PackageManager.FEATURE_CAMERA_FRONT)) {
            Log.e(TAG, "no front camera!")
            return null
        }

        val camera = Camera.open(cameraId)
        if (camera == null) {
            Log.e(TAG, "openCamera failed")
            return null
        }

        return camera
    }

    private fun haveFeature(name: String): Boolean {
        return BaseApplication.context.packageManager?.hasSystemFeature(name) ?: false
    }

    /**
     * 图片预览大小
     */
    fun setOptimalSize(camera: Camera, aspectRatio: Float, maxWidth: Int, maxHeight: Int) {
        val parameters = camera.parameters
        //使用自动对焦
        if (parameters.supportedFocusModes.contains(
                        Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
            parameters.focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE
        }
        val size = getLargePreviewSize(camera)
        size?.let {
            //设置相机预览大小
            parameters.setPreviewSize(it.width, it.height)
            Log.d(TAG, "input max: (" + maxWidth + ", " + maxHeight + "), output size: ("
                    + it.width + ", " + it.height + ")")
        }

        val pictureSize = getLargePictureSize(camera)
        pictureSize?.let {
            //图片参数
            parameters.setPictureSize(it.width, it.height)
            Log.d(TAG, "picture max: (" + maxWidth + ", " + maxHeight + "), output size: ("
                    + it.width + ", " + it.height + ")")
        }

        camera.parameters = parameters
    }

    fun chooseOptimalSize(
            options: List<Camera.Size>, aspectRatio: Float,
            maxWidth: Int, maxHeight: Int
    ): Camera.Size {
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
            return java.lang.Long.signum(
                    lhs.width.toLong() * lhs.height - rhs.width.toLong() * rhs.height)
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
        val metrics = BaseApplication.context.resources?.displayMetrics
        return metrics?.widthPixels ?: 0
    }

    fun getScreenHeight(): Int {
        val metrics = BaseApplication.context.resources?.displayMetrics
        return metrics?.heightPixels ?: 0
    }

    fun setFocusMode(camera: Camera, focusMode: String) {
        val parameters = camera.parameters
        val focusModes = parameters.supportedFocusModes
        if (focusModes.contains(focusMode)) {
            parameters.focusMode = focusMode
        }
        camera.parameters = parameters
    }

    fun getCameraInfo(camera: Camera): CameraInfo {
        val size = camera.parameters.previewSize
        val picSize = camera.parameters.pictureSize
        return CameraInfo(size.width, size.height, Camera.CameraInfo().orientation,
                cameraId == Camera.CameraInfo.CAMERA_FACING_FRONT, picSize.width, picSize.height)
    }

    /**
     * 获取最大的图片大小
     */
    fun getLargePictureSize(camera: Camera?): Camera.Size? {
        if (camera != null) {
            //获取可选比例
            val sizes = camera.parameters.supportedPictureSizes
            var temp: Camera.Size = sizes[0]
            for (i in 1 until sizes.size) {
                val scale = sizes[i].height.toFloat() / sizes[i].width
                if (temp.width < sizes[i].width && scale < 0.6f && scale > 0.5f)
                    temp = sizes[i]
            }
            return temp
        }
        return null
    }

    /**
     * 获取最大的预览大小
     */
    fun getLargePreviewSize(camera: Camera?): Camera.Size? {
        if (camera != null) {
            //获取可选比例
            Collections.sort<Camera.Size>(camera.parameters.supportedPictureSizes,
                    Comparator<Camera.Size> { o1, o2 ->
                        if (o1.width > o2.width) {
                            return@Comparator -1
                        } else if (o1.width < o2.width) {
                            return@Comparator 1
                        }
                        0
                    })

            for (size in camera.parameters.supportedPictureSizes) {
                if (size.height <= 720) {
                    //找到第一个相等或者小于width的尺寸
                    return size
                }
            }
        }
        return null
    }
}