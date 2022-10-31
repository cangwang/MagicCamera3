package com.cangwang.magic.view

import android.annotation.SuppressLint
import android.graphics.SurfaceTexture
import android.os.Build
import android.os.Environment
import android.util.Log
import android.view.Surface
import android.view.TextureView
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
class ImageFilterTextureCallback(path:String): TextureView.SurfaceTextureListener {
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= ImageFilterTextureCallback::class.java.simpleName
    private var mSurfaceTexture:SurfaceTexture?=null
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false
    private val imagePath = path

    fun changeOpenGL(width:Int,height:Int){
        mExecutor.execute {
            OpenGLJniLib.magicImageFilterChange(width,height)
            OpenGLJniLib.magicImageFilterDraw(mMatrix,"")
        }
    }

    fun drawOpenGL(surface: SurfaceTexture) {
        mExecutor.execute {
            surface.updateTexImage()
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
    fun saveImage(){
        val rootAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
        }
        Observable.create(ObservableOnSubscribe<Boolean> {
            it.onNext(OpenGLJniLib.saveImage(rootAddress))
        }).subscribeOn(Schedulers.from(mExecutor))
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

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        mExecutor.execute {
            val s = Surface(surface)
            val textureId = OpenGLJniLib.magicImageFilterCreate(s,BaseApplication.context.assets,imagePath, ExifUtil.getExifOrientation(imagePath))
            if (textureId < 0){
                Log.e(TAG, "surfaceCreated init OpenGL ES failed!")
                return@execute
            }
            changeOpenGL(width,height)
            drawOpenGL(surface)
        }
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        changeOpenGL(width,height)
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        releaseOpenGL()
        return true
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        drawOpenGL(surface)
    }
}