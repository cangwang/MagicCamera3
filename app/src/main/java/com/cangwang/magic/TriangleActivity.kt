package com.cangwang.magic

import android.opengl.GLSurfaceView
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.cangwang.filter.util.RenderJNI

/**
 * Created by zjl on 2018/10/12.
 */
class TriangleActivity: AppCompatActivity(){

    lateinit var mGLSurfaceView:GLSurfaceView
    lateinit var mRenderer: RenderJNI

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mGLSurfaceView = GLSurfaceView(this)
        mRenderer = RenderJNI(this)
        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mRenderer)
        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        setContentView(mGLSurfaceView)
    }

    override fun onResume() {
        super.onResume()
        mGLSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        mGLSurfaceView.onPause()
    }


}