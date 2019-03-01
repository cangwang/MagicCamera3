package com.cangwang.magic.video

import android.os.*
import android.util.Log
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.OpenGLJniLib
import java.io.File
import java.lang.Exception
import java.lang.ref.WeakReference
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean


class TextureMovieEncoder2:Runnable{
    private val TAG = "TextureMovieEncoder"

    private var videoEncoder:VideoEncoderCoder?=null
    private var recordThread = Executors.newSingleThreadExecutor()
    private var  isReady = AtomicBoolean()

    private val mReadyFence = java.lang.Object()      // guards ready/running
    private var mReady: Boolean = false
    private var mRunning: Boolean = false
    // ----- accessed by multiple threads -----
    @Volatile
    private var mHandler: EncoderHandler? = null

    init {
        isReady.set(false)
    }

    fun startRecord(width:Int,height:Int,textureId:Int,filterType:Int){
        Log.d(TAG, "Encoder: startRecording()")
        synchronized(mReadyFence) {
            if (mRunning) {
                Log.w(TAG, "Encoder thread already running")
                return
            }
            mRunning = true
            Thread(this, "TextureMovieEncoder").start()
            while (!mReady) {
                try {
                    mReadyFence.wait()
                } catch (ie: InterruptedException) {
                    // ignore
                }

            }
        }
        mHandler?.sendMessage(mHandler?.obtainMessage(EncoderHandler.MSG_START_RECORDING, EncoderConfig(width,height,textureId,filterType)))
    }

    fun handleStartRecord(width:Int,height:Int,textureId:Int,filterType:Int){
            if (width > 0 && height > 0) {
                videoEncoder = VideoEncoderCoder(width, height, 1000000, File(getVideoFileAddress()))
                videoEncoder?.apply {
                    OpenGLJniLib.buildVideoSurface(getInputSurface(), textureId, BaseApplication.context.assets)
                    OpenGLJniLib.magicVideoFilterChange(width,height)
                    if (filterType > 0) {
                        OpenGLJniLib.setVideoFilterType(filterType)
                    }
                    start()
                    isReady.set(true)
                }
            }

    }

    data class EncoderConfig(val width:Int,val height:Int,val textureId:Int,val filterType:Int)

    fun stopRecord(){
        mHandler?.sendMessage(mHandler?.obtainMessage(EncoderHandler.MSG_STOP_RECORDING))
        mHandler?.sendMessage(mHandler?.obtainMessage(EncoderHandler.MSG_QUIT))
    }

    fun handleStopRecord(){
            videoEncoder?.drainEncoder(true)
            videoEncoder?.release()
            videoEncoder = null
            OpenGLJniLib.releaseVideoSurface()
            isReady.set(false)

    }

    data class DrawConfig(var matrix: FloatArray,var time: Long)
    private var drawConfig:DrawConfig?=null
    fun drawFrame(matrix:FloatArray,time:Long){
        synchronized(mReadyFence) {
            if (!mReady) {
                return
            }
        }
        if (drawConfig==null){
            drawConfig =DrawConfig(matrix,time)
        }else{
            drawConfig?.matrix = matrix
            drawConfig?.time = time
        }

        mHandler?.sendMessage(mHandler?.obtainMessage(EncoderHandler.MSG_FRAME_AVAILABLE,drawConfig))
    }

    fun handleDrawFrame(matrix:FloatArray,time:Long){
        if(isReady.get()) {
                videoEncoder?.apply {
                    try {
                        drainEncoder(false)
                    }catch (e:Exception){
                        Log.e(TAG,e.toString())
                    }
                    OpenGLJniLib.magicVideoDraw(matrix, time)
                }

        }
    }

    private fun getVideoFileAddress(): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".mp4"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".mp4"
        }
    }

    override fun run() {
        Looper.prepare()
        synchronized(mReadyFence) {
            mHandler = EncoderHandler(this)
            mReady = true
            mReadyFence.notify()
        }
        Looper.loop()

        Log.d(TAG, "Encoder thread exiting")
        synchronized(mReadyFence) {
            mRunning = false
            mReady = mRunning
            mHandler = null
        }
    }

    class EncoderHandler(encoder: TextureMovieEncoder2) : Handler() {

        companion object {
            val TAG = "EncoderHandler"
            val MSG_START_RECORDING = 0
            val MSG_STOP_RECORDING = 1
            val MSG_FRAME_AVAILABLE = 2
            val MSG_SET_TEXTURE_ID = 3
            val MSG_UPDATE_SHARED_CONTEXT = 4
            val MSG_QUIT = 5
        }
        private val mWeakEncoder: WeakReference<TextureMovieEncoder2> = WeakReference(encoder)

        override// runs on encoder thread
        fun handleMessage(inputMessage: Message) {
            val what = inputMessage.what
            val obj = inputMessage.obj

            val encoder = mWeakEncoder.get()
            if (encoder == null) {
                Log.w(TAG, "EncoderHandler.handleMessage: encoder is null")
                return
            }

            when (what) {
                MSG_START_RECORDING -> {
                    val config = obj as EncoderConfig
                    encoder.handleStartRecord(config.width,config.height,config.textureId,config.filterType)
                }
                MSG_STOP_RECORDING -> encoder.handleStopRecord()
                MSG_FRAME_AVAILABLE -> {
                    val config = obj as DrawConfig
                    encoder.handleDrawFrame(config.matrix,config.time)
                }
                MSG_QUIT -> Looper.myLooper()!!.quit()
                else -> throw RuntimeException("Unhandled msg what=$what")
            }
        }
    }
}