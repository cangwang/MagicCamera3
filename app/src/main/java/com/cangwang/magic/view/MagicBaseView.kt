package com.cangwang.magic.view

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * Created by zjl on 2018/10/12.
 */
class MagicBaseView(context: Context,attributeSet: AttributeSet):GLSurfaceView(context,attributeSet),GLSurfaceView.Renderer{

    init {

    }

    override fun onDrawFrame(p0: GL10?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    override fun onSurfaceChanged(p0: GL10?, p1: Int, p2: Int) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }


}