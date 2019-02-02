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
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
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
    private var mCamera=camera
    private val mMatrix = FloatArray(16)
    private var width = 0
    private var height = 0
    private var isTakePhoto = false

    private var mMediaRecorder:MediaRecorder?=null
    private var isRecordVideo = AtomicBoolean()
    private var previewSurface:Surface?=null

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

    fun startVideoRecord(path:String):Boolean{
        if (isRecordVideo.get()){
            Log.e(TAG,"video is recording")
            return false
        }
        if (path.isEmpty()){
            Log.e(TAG,"record path is empty")
            return false
        }
        if (!setMediaRecordParam(path)){
            Log.e(TAG,"record path is empty")
            return false
        }
        startRecordVideo()
        return true
    }

    fun setMediaRecordParam(path:String):Boolean{
        mMediaRecorder = MediaRecorder()
        mMediaRecorder?.apply {
            setAudioSource(MediaRecorder.AudioSource.MIC)
            setVideoSource(MediaRecorder.VideoSource.SURFACE)
            setOutputFormat(MediaRecorder.OutputFormat.MPEG_4)
            setOutputFile(path)
            val bitRate= width*height
            setVideoEncodingBitRate(bitRate)
            setVideoSize(width,height)
            setVideoEncoder(MediaRecorder.VideoEncoder.H264)

            setAudioEncodingBitRate(8000)
            setAudioChannels(1)
            setAudioSamplingRate(8000)
            setAudioEncodingBitRate(MediaRecorder.AudioEncoder.AAC)
            if (mCamera?.getCameraType() == CameraCompat.BACK_CAMERA){
                setOrientationHint(90)
            }else{
                setOrientationHint(270)
            }
            try {
                prepare()
            }catch (e:IOException){
                e.printStackTrace()
                return false
            }
        }
        return true
    }

    fun startRecordVideo(){
        isRecordVideo.set(true)
        mMediaRecorder?.surface?.let {
            initOpenGL(it)
        }
    }

    fun stopRecordVideo(){
        if(isRecordVideo.get()){
           mMediaRecorder?.stop()
        }
    }

    fun relaseRecordVideo(){
        mMediaRecorder?.apply {
            stop()
            reset()
            release()
        }
        if (isRecordVideo.get()){
            isRecordVideo.set(false)
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
                val photoAddress = if(Build.BRAND == "Xiaomi"){ // 小米手机
                    Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".png"
                }else{  // Meizu 、Oppo
                    Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".png"
                }
                OpenGLJniLib.magicFilterDraw(mMatrix,photoAddress)
                isTakePhoto =false
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