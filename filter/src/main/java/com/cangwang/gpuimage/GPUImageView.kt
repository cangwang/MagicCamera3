package com.cangwang.gpuimage

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.widget.FrameLayout

class GPUImageView : FrameLayout, GPUImageTarget {

    companion object {
        internal val FillModeStretch =
                0                   // Stretch to fill the view, and may distort the image
        internal val FillModePreserveAspectRatio = 1       // preserve the aspect ratio of the image
        internal val FillModePreserveAspectRatioAndFill =
                2 // preserve the aspect ratio, and zoom in to fill the view
    }

    private var nativeClassID = 0L

    private var mGLSurfaceView: GLSurfaceView? = null

    val surfaceWidth: Int
        get() = mGLSurfaceView!!.width

    val surfaceHeight: Int
        get() = mGLSurfaceView!!.height

    constructor(context: Context) : super(context) {
        init(context, null)
    }

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs) {
        init(context, attrs)
    }

    private fun init(context: Context, attrs: AttributeSet?) {
        if (nativeClassID != 0L) return
        GPUImage.init()
        GPUImage.runOnDraw(
                Runnable { nativeClassID = GPUImage.nativeTargetViewNew() })

        mGLSurfaceView = GPUImageViewGLSurfaceView(context, attrs, this)
        mGLSurfaceView?.let {
            GPUImage.setGLSurfaceView(it)
        }

        addView(mGLSurfaceView)
        if (mGLSurfaceView!!.width != 0 && mGLSurfaceView!!.height != 0) {
            onSurfaceSizeChanged(mGLSurfaceView!!.width, mGLSurfaceView!!.height)
        }
    }

    override fun getNativeClassID(): Long {
        return nativeClassID
    }

    protected fun onSurfaceSizeChanged(w: Int, h: Int) {
        GPUImage.runOnDraw(Runnable {
            if (nativeClassID != 0L)
                GPUImage.nativeTargetViewOnSizeChanged(nativeClassID, w, h)
        })
    }

    fun setFillMode(fillMode: Int) {
        GPUImage.runOnDraw(Runnable {
            if (nativeClassID != 0L)
                GPUImage.nativeTargetViewSetFillMode(nativeClassID, fillMode)
        })
    }

    private inner class GPUImageViewGLSurfaceView(
            context: Context, attrs: AttributeSet?, private val host: GPUImageView
    ) : GLSurfaceView(context, attrs) {

        override fun surfaceCreated(holder: SurfaceHolder) {
            super.surfaceCreated(holder)
        }

        override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
            super.surfaceChanged(holder, format, w, h)
            host.onSurfaceSizeChanged(w, h)
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
            super.surfaceDestroyed(holder)
        }
    }

//    override fun onTouchEvent(event: MotionEvent?): Boolean {
//        //一个手指操作
//        if (event?.pointerCount == 1) {
//            consumeTouchEvent(event)
//        }
//        return super.onTouchEvent(event)
//    }
//
//    private fun consumeTouchEvent(e: MotionEvent) {
//        var touchX = -1f
//        var touchY = -1f
//        when (e.action) {
//            MotionEvent.ACTION_MOVE -> {
//                touchX = e.x
//                touchY = e.y
//            }
//            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
//                touchX = -1f
//                touchY = -1f
//            }
//        }
////        when (mGLRender.getSampleType()) {
////            SAMPLE_TYPE_KEY_SCRATCH_CARD -> {
////                mGLRender.setTouchLoc(touchX, touchY)
////                requestRender()
////            }
////            else -> {
////            }
////        }
//        GPUImage.setTouchLocation(nativeClassID, touchX, touchY)
//        GPUImage.requestRender()
//    }
}
