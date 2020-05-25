package com.cangwang.magic.view


import android.annotation.SuppressLint
import android.graphics.SurfaceTexture
import android.media.MediaRecorder
import android.os.Build
import android.os.Environment
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.widget.Toast
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.camera.CameraCompat
import com.cangwang.filter.util.OpenGLJniLib
import com.cangwang.magic.video.VideoEncoderCoderMP4
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.io.File
import java.io.IOException

import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by zjl on 2018/10/12.
 */
class CameraFilterSurfaceCallbackV2(camera:CameraCompat?):SurfaceHolder.Callback{
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= CameraFilterSurfaceCallbackV2::class.java.simpleName!!
    private var mSurfaceTexture:SurfaceTexture?=null
    private var mSurface:Surface?=null
    private var mCamera=camera
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false

    private var mMediaRecorder:MediaRecorder?=null
    private var isRecordVideo = AtomicBoolean()
    private var previewSurface:Surface?=null
    private var videoEncoder:VideoEncoderCoderMP4 ?=null

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        this.width = width
        this.height = height
        changeOpenGL(width,height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        mCamera?.stopPreview(true)
        releaseOpenGL()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        holder?.let {
            previewSurface = it.surface
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
            mSurface = Surface(mSurfaceTexture)
            try {
                mSurfaceTexture?.let {
                    mCamera?.setSurfaceTexture(it)
                }
                doStartPreview()
            }catch (e:IOException){
                Log.e(TAG,e.localizedMessage)
                releaseOpenGL()
            }
        }
    }

    fun startRecordVideo(){
        if (isRecordVideo.get()){
            return
        }

        if (mSurface!=null && width>0 && height>0)
            videoEncoder = VideoEncoderCoderMP4(width,height,1000000, File(getVideoFileAddress()))

        videoEncoder?.start()
        isRecordVideo.set(true)
    }

    fun stopRecordVideo(){
        videoEncoder?.stop()
    }

    fun resumeRecordVideo(){

    }

    fun isRecording():Boolean{
        return isRecordVideo.get()
    }

    fun releaseRecordVideo(){
        mExecutor.execute {
            if (isRecordVideo.get()) {
                videoEncoder?.drainEncoder(true)
                videoEncoder?.release()
                videoEncoder = null
                isRecordVideo.set(false)
            }
        }
    }

    fun changeCamera(camera:CameraCompat? ){
        mExecutor.execute {
            mCamera = camera
            try {
                mSurfaceTexture?.let {
                    mCamera?.setSurfaceTexture(it)
                }
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
                val photoAddress = getImageFileAddress()
                OpenGLJniLib.magicFilterDraw(mMatrix,photoAddress)
                isTakePhoto =false
            }else {
                OpenGLJniLib.magicFilterDraw(mMatrix,"")
            }
            videoEncoder?.drainEncoder(false)
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

    fun setFilterType(type:Int){
        mExecutor.execute {
            OpenGLJniLib.setFilterType(type)
        }
    }

    fun doStartPreview(){
        mCamera?.startPreview(object :CameraCompat.CameraStateCallBack{
            override fun onConfigured() {
                if (isRecordVideo.get()){
                    mMediaRecorder?.start()
                }
            }

            override fun onConfigureFailed() {

            }
        })
    }

    @SuppressLint("CheckResult")
    fun takePhoto(){
        val rootAddress= getImageFileAddress()
//        mCamera?.stopPreview()
        Observable.create(ObservableOnSubscribe<Boolean> {
            it.onNext(OpenGLJniLib.savePhoto(rootAddress))
        }).subscribeOn(Schedulers.from(mExecutor))
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

    fun getVideoFileAddress(): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".mp4"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".mp4"
        }
    }

    fun getImageFileAddress():String{
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
        }
    }
}