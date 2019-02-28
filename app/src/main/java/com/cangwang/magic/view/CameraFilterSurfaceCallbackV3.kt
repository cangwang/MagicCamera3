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
import com.cangwang.magic.util.OpenGLJniLib
import com.cangwang.magic.video.VideoEncoderCoder
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
class CameraFilterSurfaceCallbackV3(camera:CameraCompat?):SurfaceHolder.Callback{
    private val mExecutor = Executors.newSingleThreadExecutor()

    private val TAG= CameraFilterSurfaceCallbackV3::class.java.simpleName!!
    private var mSurfaceTexture:SurfaceTexture?=null
    private var mSurface:Surface?=null
    private var mCamera=camera
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false
    private var textureId:Int = -1

    private var mMediaRecorder:MediaRecorder?=null
    private var isRecordVideo = AtomicBoolean()
    private var previewSurface:Surface?=null
    private var videoEncoder:VideoEncoderCoder ?=null
    private var recordStatus = RECORD_IDLE

    companion object {
        val RECORD_IDLE = 0
        val RECORD_START = 1
        val RECORD_RECORDING = 2
        val RECORD_STOP = 3
    }

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
            mSurface = it.surface
        }
    }

    fun initOpenGL(surface: Surface){
        mExecutor.execute {
            textureId = OpenGLJniLib.magicFilterCreate(surface,BaseApplication.context.assets)
//            OpenGLJniLib.setFilterType(MagicFilterType.NONE.ordinal)
            if (textureId < 0){
                Log.e(TAG, "surfaceCreated init OpenGL ES failed!")
                return@execute
            }
            mSurfaceTexture = SurfaceTexture(textureId)
            mSurfaceTexture?.setOnFrameAvailableListener { drawOpenGL() }
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
        recordStatus = RECORD_START
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
        recordStatus = RECORD_STOP
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
            mSurfaceTexture?.let{
                it.updateTexImage()

                if(recordStatus == RECORD_START) {
                    if (width > 0 && height > 0)
                        videoEncoder = VideoEncoderCoder(width, height, 1000000, File(getVideoFileAddress()))
                    videoEncoder?.apply {
                        OpenGLJniLib.buildVideoSurface(getInputSurface(),textureId,BaseApplication.context.assets)
                        start()
                        isRecordVideo.set(true)
                    }
                    recordStatus = RECORD_RECORDING
                }else if (recordStatus == RECORD_STOP){
                    if (isRecordVideo.get()) {
                        videoEncoder?.drainEncoder(true)
                        videoEncoder?.release()
                        videoEncoder = null
                        OpenGLJniLib.releaseVideoSurface()
                        isRecordVideo.set(false)
                        recordStatus = RECORD_IDLE
                    }
                }

                it.getTransformMatrix(mMatrix)
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