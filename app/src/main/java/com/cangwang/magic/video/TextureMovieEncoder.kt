package com.cangwang.magic.video

import android.os.Build
import android.os.Environment
import android.util.Log
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.OpenGLJniLib
import java.io.File
import java.lang.Exception
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean


class TextureMovieEncoder{
    private val TAG = "TextureMovieEncoder"
    private var videoEncoder:VideoEncoderCoder?=null
    private var recordThread = Executors.newSingleThreadExecutor()
    private var isReady = AtomicBoolean()

    init {
        isReady.set(false)
    }

    fun startRecord(width:Int,height:Int,textureId:Int,filterType:Int){
        recordThread.execute {
            if (width > 0 && height > 0) {
                videoEncoder = VideoEncoderCoder(width, height, width*height*2, File(getVideoFileAddress()))
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
    }

    fun stopRecord(){
        recordThread.execute {
            videoEncoder?.drainEncoder(true)
            videoEncoder?.release()
            videoEncoder = null
            OpenGLJniLib.releaseVideoSurface()
            isReady.set(false)
        }
    }

    fun drawFrame(matrix:FloatArray,time:Long){
        if(isReady.get()) {
            recordThread.execute {
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
    }

    private fun getVideoFileAddress(): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+System.currentTimeMillis()+".mp4"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+System.currentTimeMillis()+".mp4"
        }
    }
}