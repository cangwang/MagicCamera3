package com.cangwang.magic.view

import android.annotation.SuppressLint
import android.graphics.SurfaceTexture
import android.os.Build
import android.os.Environment
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.widget.Toast
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.ExifUtil
import com.cangwang.magic.util.OpenGLJniLib
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers

import java.util.concurrent.Executors

/**
 * Created by zjl on 2018/10/12.
 */
class ImageFilterSurfaceCallback(path:String):SurfaceHolder.Callback{
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= ImageFilterSurfaceCallback::class.java.simpleName!!
    private var mSurfaceTexture:SurfaceTexture?=null
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false
    private val imagePath = path

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

    private fun initOpenGL(surface: Surface){
        mExecutor.execute {
            val textureId = OpenGLJniLib.magicImageFilterCreate(surface,BaseApplication.context.assets,imagePath,ExifUtil.getExifOrientation(imagePath))
            if (textureId < 0){
                Log.e(TAG, "surfaceCreated init OpenGL ES failed!")
                return@execute
            }
            mSurfaceTexture = SurfaceTexture(textureId)
            mSurfaceTexture?.setOnFrameAvailableListener {
                drawOpenGL()
            }

        }
    }


    fun changeOpenGL(width:Int,height:Int){
        mExecutor.execute {
            OpenGLJniLib.magicImageFilterChange(width,height)
            OpenGLJniLib.magicImageFilterDraw(mMatrix,"")
        }
    }

    fun drawOpenGL(){
        mExecutor.execute {
            mSurfaceTexture?.updateTexImage()
//            mSurfaceTexture?.getTransformMatrix(mMatrix)
            if (isTakePhoto){
                val photoAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
                    Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
                }else{  // Meizu 、Oppo
                    Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
                }
                OpenGLJniLib.magicImageFilterDraw(mMatrix,photoAddress)
            }else {
                OpenGLJniLib.magicImageFilterDraw(mMatrix,"")
            }
        }
    }

    private fun releaseOpenGL(){
        mExecutor.execute {
            OpenGLJniLib.magicImageFilterRelease()
            mSurfaceTexture?.release()
            mSurfaceTexture=null
        }
    }

    fun setFilterType(type:Int){
        mExecutor.execute {
            OpenGLJniLib.setImageFilterType(type)
            OpenGLJniLib.magicImageFilterDraw(mMatrix,"")
        }
    }

    @SuppressLint("CheckResult")
    fun savePhoto(){
        val rootAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
        }
        Observable.create(ObservableOnSubscribe<Boolean> {
            it.onNext(OpenGLJniLib.savePhoto(rootAddress))
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe({
                    if (!it){
                        Toast.makeText(BaseApplication.context,"save fail",Toast.LENGTH_SHORT).show()
                    }else{
                        Toast.makeText(BaseApplication.context,"save success",Toast.LENGTH_SHORT).show()
                    }
                },{
                    Log.e(TAG,it.toString())
                })
    }
}