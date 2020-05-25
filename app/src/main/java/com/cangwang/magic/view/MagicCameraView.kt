//package com.cangwang.magic.view
//
//import android.content.Context
//import android.opengl.GLSurfaceView
//import android.util.AttributeSet
//import OpenGLJniLib
//import javax.microedition.khronos.egl.EGLConfig
//import javax.microedition.khronos.opengles.GL10
//
///**
// * 相机渲染
// * Created by cangwang on 2018/10/12.
// */
//class MagicCameraView(context: Context, attributeSet: AttributeSet):GLSurfaceView(context,attributeSet),GLSurfaceView.Renderer{
//
//    init {
//        OpenGLJniLib.magicBaseInit()
//        setEGLContextClientVersion(3)
//        setRenderer(this)
//        renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
//    }
//
//    override fun onDrawFrame(p0: GL10?) {
//        OpenGLJniLib.magicBaseDraw()
//    }
//
//    override fun onSurfaceChanged(p0: GL10?, width: Int, height: Int) {
//        OpenGLJniLib.magicBaseChange(width,height)
//    }
//
//    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
//        OpenGLJniLib.magicBaseCreate()
//    }
//}