package com.cangwang.magic.view


import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.CameraHelper
import com.cangwang.magic.util.OpenGLJniLib
import java.io.IOException
import java.util.concurrent.Executors

/**
 * Created by zjl on 2018/10/12.
 */
class CameraSurfaceCallback(camera:Camera?):SurfaceHolder.Callback{
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= CameraSurfaceCallback::class.java.simpleName!!
    private var mSurfaceTexture:SurfaceTexture?=null
    private val mCamera=camera
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        this.width = width
        this.height = height
        holder?.let {
            initOpenGL(it.surface,width,height)
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        releaseOpenGL()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {

    }

    fun initOpenGL(surface: Surface, width: Int, height: Int){
        mExecutor.execute {
            val textureId = OpenGLJniLib.magicBaseInit(surface,width,height,BaseApplication.context.assets)
            if (textureId < 0){
                Log.e(TAG, "surfaceCreated init OpenGL ES failed!")
                return@execute
            }
            mSurfaceTexture = SurfaceTexture(textureId)
            mSurfaceTexture?.setOnFrameAvailableListener { drawOpenGL() }
            try {
                mCamera?.setPreviewTexture(mSurfaceTexture)
                doStartPreview()
            }catch (e:IOException){
                Log.e(TAG,e.localizedMessage)
                releaseOpenGL()
            }
        }
    }

    fun drawOpenGL(){
        mExecutor.execute {
            mSurfaceTexture?.updateTexImage()
            mSurfaceTexture?.getTransformMatrix(mMatrix)
            OpenGLJniLib.magicBaseDraw(mMatrix)
        }
    }

    fun releaseOpenGL(){
        mExecutor.execute {
            mSurfaceTexture?.release()
            mSurfaceTexture=null
            OpenGLJniLib.magicBaseRelease()
        }
    }

    fun doStartPreview(){
        mCamera?.startPreview()
        cameraFocus(width/2.0f,height/2.0f)
    }

    fun cameraFocus(x:Float,y:Float){
        mCamera?.let {
            it.cancelAutoFocus()
            CameraHelper.setFocusMode(it, Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)
        }
    }
}