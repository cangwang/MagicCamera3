package com.cangwang.gpuimage

import android.content.Context
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.opengl.GLES20
import android.os.Build
import android.util.Log
import android.view.Surface
import android.view.WindowManager
import java.io.IOException
import java.nio.IntBuffer

class GPUImageSourceCamera(context: Context) : GPUImageSource(), Camera.PreviewCallback {
    private var mCamera: Camera? = null
    private var mCurrentCameraId = 0
    private var mRGBABuffer: IntBuffer? = null
    private var mRotation = GPUImage.NoRotation
    private val mContext = context
    private var mSurfaceTexture: SurfaceTexture? = null

    init {
        if (mNativeClassID != 0L) {
            GPUImage.runOnDraw(Runnable {
                mNativeClassID = GPUImage.nativeSourceCameraNew()
            })
        }
    }

    override fun onPreviewFrame(data: ByteArray?, camera: Camera?) {
        val previewSize = camera?.parameters?.previewSize
        if (mRGBABuffer == null && previewSize != null) {
            mRGBABuffer = IntBuffer.allocate(previewSize.width * previewSize.height)
        }

        val cam = camera
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L
                    && data != null && data.isNotEmpty() && previewSize != null) {
                GPUImage.nativeYUVtoRBGA(data, previewSize.width, previewSize.height,
                        mRGBABuffer!!.array())
                cam?.addCallbackBuffer(data)
                GPUImage.nativeSourceCameraSetFrame(mNativeClassID, previewSize.width,
                        previewSize.height, mRGBABuffer!!.array(), mRotation)
            }
        })
        proceed()
    }

    fun onResume() {
        setUpCamera(mCurrentCameraId)
    }

    fun onPause() {
        releaseCamera()
    }

    fun switchCamera() {
        releaseCamera()
        mCurrentCameraId = (mCurrentCameraId + 1) % Camera.getNumberOfCameras()
        setUpCamera(mCurrentCameraId)
    }

    fun setUpCamera(id: Int) {
        mCamera = Camera.open(id)
        val parameters = mCamera?.parameters
        //设置相机连续采样
        if (parameters != null && parameters.supportedFocusModes.contains((Camera.Parameters
                        .FOCUS_MODE_CONTINUOUS_PICTURE))) {
            parameters.focusMode = Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE
        }
        mCamera?.parameters = parameters

        //获取屏幕角度
        val deviceRotation =
                (mContext.getSystemService(Context.WINDOW_SERVICE) as WindowManager)
                        .defaultDisplay.rotation
        val info = Camera.CameraInfo()
        Camera.getCameraInfo(mCurrentCameraId, info)

        var rotation = 0
        when (deviceRotation) {
            Surface.ROTATION_0 -> rotation = 0
            Surface.ROTATION_90 -> rotation = 90
            Surface.ROTATION_180 -> rotation = 180
            Surface.ROTATION_270 -> rotation = 270
        }

        mRotation = GPUImage.NoRotation
        //前置摄像角度加入翻转
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            rotation = (info.orientation + rotation) % 360
            when (rotation) {
                0 -> mRotation = GPUImage.FlipHorizontal
                90 -> mRotation = GPUImage.RotateRightFlipVertical
                180 -> mRotation = GPUImage.FlipVertical
                270 -> mRotation = GPUImage.RotateRightFlipHorizontal
            }
        } else { //后置摄像角度
            rotation = (info.orientation - rotation + 360) % 360
            when (rotation) {
                90 -> mRotation = GPUImage.RotateRight
                180 -> mRotation = GPUImage.Rotate180
                270 -> mRotation = GPUImage.RotateLeft
            }
        }

        if (Build.VERSION.SDK_INT > Build.VERSION_CODES.GINGERBREAD_MR1) {
            GPUImage.runOnDraw(Runnable {
                if (mNativeClassID != 0L) {
                    val textures = IntArray(1)
                    GLES20.glGenTextures(1, textures, 0)
                    mSurfaceTexture = SurfaceTexture(textures[0])
                    try {
                        mCamera?.setPreviewTexture(mSurfaceTexture)
                        mCamera?.setPreviewCallback(this@GPUImageSourceCamera)
                        mCamera?.startPreview()
                    } catch (e: IOException) {
                        Log.e("GPUImageSourceCamera", e.toString())
                    }
                }
            })
            GPUImage.requestRender()
        } else {
            mCamera?.setPreviewCallback(this)
            mCamera?.startPreview()
        }
    }

    fun releaseCamera() {
        mSurfaceTexture = null
        mCamera?.setPreviewCallback(null)
        mCamera?.release()
        mCamera = null
    }

    fun destroy() {
        destroy(true)
    }

    fun destroy(onGLThread: Boolean) {
        if (mNativeClassID != 0L) {
            if (onGLThread) {
                GPUImage.runOnDraw(Runnable {
                    if (mNativeClassID != 0L) {
                        GPUImage.nativeSourceCameraDestroy(mNativeClassID)
                        mNativeClassID = 0
                    }
                })
            } else {
                GPUImage.nativeSourceCameraDestroy(mNativeClassID)
                mNativeClassID = 0
            }
        }
    }
}