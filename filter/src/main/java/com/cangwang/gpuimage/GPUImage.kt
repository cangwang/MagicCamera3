package com.cangwang.gpuimage

import android.annotation.SuppressLint
import android.graphics.Bitmap
import android.graphics.PixelFormat
import android.opengl.GLSurfaceView
import android.os.Build

@SuppressLint("StaticFieldLeak")
object GPUImage {
    val NoRotation = 0
    val RotateLeft = 1
    val RotateRight = 2
    val FlipVertical = 3
    val FlipHorizontal = 4
    val RotateRightFlipVertical = 5
    val RotateRightFlipHorizontal = 6
    val Rotate180 = 7

    private var mRenderer: GPUImageRenderer? = null
    private var mGLSurfaceView: GLSurfaceView? = null
    private var mGLSurfaceViewRenderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

    init {
        System.loadLibrary("GPUImage-x")
        init()
    }

    fun init() {
        if (!isInited()) {
            mRenderer = GPUImageRenderer()
            runOnDraw(Runnable {
                nativeContextInit()
            })
        }
    }

    private fun isInited(): Boolean {
        return mRenderer != null
    }

    fun destroy() {
        if (!isInited()) return
        purge()
        setGLSurfaceView(null)
        setSource(null)
        mRenderer?.clear()
        mRenderer = null
    }

    fun setSource(source: GPUImageSource?) {
        mRenderer?.setSource(source)
    }

    fun setGLSurfaceView(view: GLSurfaceView?) {
        mGLSurfaceView = view
        if (mGLSurfaceView != null) {
            mGLSurfaceView?.setEGLContextClientVersion(3)
            mGLSurfaceView?.setEGLConfigChooser(8, 8, 8, 8, 16, 0)
            mGLSurfaceView?.holder?.setFormat(PixelFormat.RGBA_8888)
            mGLSurfaceView?.setRenderer(mRenderer)
            mGLSurfaceView?.renderMode = mGLSurfaceViewRenderMode
            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.GINGERBREAD_MR1) {
                mGLSurfaceView?.preserveEGLContextOnPause = true
            }
            mGLSurfaceView?.requestRender()
        } else {
            mGLSurfaceViewRenderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        }
    }

    fun runOnDraw(runnable: Runnable) {
        mRenderer?.runOnDraw(runnable)
    }

    fun purge() {
        if (mGLSurfaceView != null) {
            runOnDraw(Runnable {
                nativeContextPurge()
            })
            mGLSurfaceView?.requestRender()
        } else {
            nativeContextPurge()
        }
    }

    fun requestRender() {
        mGLSurfaceView?.requestRender()
    }

    fun getRenderer(): GPUImageRenderer? {
        return mRenderer
    }

    fun isPreDrawQueueEmpty(): Boolean {
        if (mRenderer != null) {
            return mRenderer!!.isPreDrawQueueEmpty()
        }
        return false
    }

    fun runOnPreDraw(runnable: Runnable) {
        mRenderer?.runOnPreDraw(runnable)
    }

    fun isDrawQueueEmpty(): Boolean {
        if (mRenderer != null) {
            return mRenderer!!.isDrawQueueEmpty()
        }
        return false
    }

    fun isPostDrawQueueEmpty(): Boolean {
        if (mRenderer != null) {
            return mRenderer!!.isPostDrawQueueEmpty()
        }
        return false
    }

    fun runOnPostDraw(runnable: Runnable) {
        mRenderer?.runOnPostDraw(runnable)
    }

    // Filter
    external fun nativeFilterCreate(filterClassName: String): Long

    external fun nativeFilterDestroy(classID: Long)
    external fun nativeFilterFinalize(classID: Long)
    external fun nativeFilterSetPropertyFloat(classID: Long, property: String, value: Float)
    external fun nativeFilterSetPropertyInt(classID: Long, property: String, value: Int)
    external fun nativeFilterSetPropertyString(classID: Long, prooerty: String, value: String)

    // SourceImage
    external fun nativeSourceImageNew(): Long

    external fun nativeSourceImageDestroy(classID: Long)
    external fun nativeSourceImageFinalize(classID: Long)
    external fun nativeSourceImageSetImage(classID: Long, bitmap: Bitmap)

    // SourceCamera
    external fun nativeSourceCameraNew(): Long

    external fun nativeSourceCameraDestroy(classID: Long)
    external fun nativeSourceCameraFinalize(classID: Long)
    external fun nativeSourceCameraSetFrame(
            classID: Long, width: Int, height: Int, data: IntArray, rotation: Int
    )

    // Source
    external fun nativeSourceAddTarget(
            classID: Long, targetClassID: Long, texID: Int, isFilter: Boolean
    ): Long

    external fun nativeSourceRemoveTarget(
            classID: Long, targetClassID: Long, isFilter: Boolean
    ): Boolean

    external fun nativeSourceRemoveAllTargets(classID: Long): Boolean
    external fun nativeSourceProceed(classID: Long, bUpdateTargets: Boolean): Boolean
    external fun nativeSourceGetRotatedFramebuferWidth(classID: Long): Int
    external fun nativeSourceGetRotatedFramebuferHeight(classID: Long): Int
    external fun nativeSourceCaptureAProcessedFrameData(
            classId: Long, upToFilterClassId: Long, width: Int, height: Int
    ): ByteArray?

    // view
    external fun nativeTargetViewNew(): Long

    external fun nativeTargetViewFinalize(classID: Long)
    external fun nativeTargetViewOnSizeChanged(classID: Long, width: Int, height: Int)
    external fun nativeTargetViewSetFillMode(classID: Long, fillMode: Int)
    // context
    external fun nativeContextInit()

    external fun nativeContextDestroy()
    external fun nativeContextPurge()

    // utils
    external fun nativeYUVtoRBGA(yuv: ByteArray, width: Int, height: Int, out: IntArray)

    //设置点击坐标
    external fun setTouchLocation(classID: Long, x: Float, y: Float): Boolean
}