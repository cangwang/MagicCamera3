package com.cangwang.magic.util

import android.content.Context
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * Created by cangwang on 2018/10/12.
 */
class RenderJNI(context: Context):GLSurfaceView.Renderer{
    companion object {
        init {
            System.loadLibrary("triangle-lib")
        }
    }

    external fun glesInit()
    external fun glesRender()
    external fun glesResize(width:Int,height:Int)
    external fun readShaderFile(assetManager: AssetManager)

    val assetMaanager:AssetManager = context.assets


    override fun onDrawFrame(p0: GL10?) {
        glesRender()
    }

    override fun onSurfaceChanged(p0: GL10?, width: Int, height: Int) {
        glesResize(width,height)
    }

    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
        readShaderFile(assetMaanager)
        glesInit()
    }

}