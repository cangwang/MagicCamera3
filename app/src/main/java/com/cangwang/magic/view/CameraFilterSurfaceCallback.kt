package com.cangwang.magic.view


import android.annotation.SuppressLint
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.os.Build
import android.os.Environment
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.widget.Toast
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.CameraHelper
import com.cangwang.magic.util.OpenGLJniLib
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.io.IOException
import java.util.*
import java.util.concurrent.Executors

/**
 * Created by zjl on 2018/10/12.
 */
class CameraFilterSurfaceCallback(camera:Camera?):SurfaceHolder.Callback{
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= CameraFilterSurfaceCallback::class.java.simpleName!!
    private var mSurfaceTexture:SurfaceTexture?=null
    private var mCamera=camera
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        this.width = width
        this.height = height
        changeOpenGL(width,height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        releaseOpenGL()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        holder?.let {
            initOpenGL(it.surface)
        }
    }

    fun initOpenGL(surface: Surface){
        mExecutor.execute {
            val textureId = OpenGLJniLib.magicFilterCreate(surface,BaseApplication.context.assets)
//            OpenGLJniLib.setFilterType(MagicFilterType.NONE.ordinal)
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

    fun changeCamera(camera:Camera? ){
        mExecutor.execute {
            mCamera = camera
            try {
                mCamera?.setPreviewTexture(mSurfaceTexture)
                doStartPreview()
            } catch (e: IOException) {
                Log.e(TAG, e.localizedMessage)
                releaseOpenGL()
            }
        }
    }

    fun changeOpenGL(width:Int,height:Int){
        mExecutor.execute {
            OpenGLJniLib.magicFilterChange(width,height)
        }
    }

    fun drawOpenGL(){
        mExecutor.execute {
            mSurfaceTexture?.updateTexImage()
            mSurfaceTexture?.getTransformMatrix(mMatrix)
            if (isTakePhoto){
                val photoAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
                    Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
                }else{  // Meizu 、Oppo
                    Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
                }
                OpenGLJniLib.magicFilterDraw(mMatrix,photoAddress)
            }else {
                OpenGLJniLib.magicFilterDraw(mMatrix,"")
            }
        }
    }

    fun releaseOpenGL(){
        mExecutor.execute {
            OpenGLJniLib.magicFilterRelease()
            mSurfaceTexture?.release()
            mSurfaceTexture=null
            mCamera =null
        }
    }

    fun setsetFilterType(type:Int){
        mExecutor.execute {
            OpenGLJniLib.setFilterType(type)
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

    @SuppressLint("CheckResult")
    fun takePhoto(){
        val rootAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
        }
//        mCamera?.stopPreview()
        Observable.create(ObservableOnSubscribe<Boolean> {
            it.onNext(OpenGLJniLib.savePhoto(rootAddress))
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe({
//                    mCamera?.startPreview()
                    if (!it){
                        Toast.makeText(BaseApplication.context,"save fail",Toast.LENGTH_SHORT).show()
                    }else{
                        Toast.makeText(BaseApplication.context,"save success",Toast.LENGTH_SHORT).show()
                    }
                },{
//                    mCamera?.startPreview()
                    Log.e(TAG,it.toString())
                })
    }
}