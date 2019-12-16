package com.cangwang.magic.video

import android.os.Build
import android.os.Environment
import android.util.Log
import com.cangwang.filter.util.OpenGLJniLib
import com.cangwang.magic.BaseApplication
import java.io.File
import java.lang.Exception
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by cangwang on 2019.3.6
 * 分别录制aac音频和mp4视频
 */
class TextureMovieEncoderSource{
    private val TAG = "TextureMovieEncoder"
    private var videoEncoder:VideoEncoderCoderMP4?=null
    private var recordThread = Executors.newSingleThreadExecutor()
    private var isReady = AtomicBoolean()
    private var audioEncoder:AudioRecordCoderAAC?=null

    init {
        isReady.set(false)
    }

    fun startRecord(width:Int,height:Int,textureId:Int,filterType:Int){
        val time = System.currentTimeMillis()
        recordThread.execute {
            if (width > 0 && height > 0) {
                //码率为4*高*宽，使用滤镜，太低码率无法记录足够的细节
                videoEncoder = VideoEncoderCoderMP4(width, height, width*height*4, File(getVideoFileAddress(time)))
                videoEncoder?.apply {
                    OpenGLJniLib.buildVideoSurface(getInputSurface(), textureId, BaseApplication.context.assets)
                    OpenGLJniLib.magicVideoFilterChange(width,height)
                    if (filterType >= 0) {
                        OpenGLJniLib.setVideoFilterType(filterType)
                    }
                    start()
                    isReady.set(true)
                }
            }
        }
        audioEncoder = AudioRecordCoderAAC()
        audioEncoder?.start(getAudioFileAddress(time))
    }

    fun stopRecord(){
        recordThread.execute {
            videoEncoder?.drainEncoder(true)
            videoEncoder?.release()
            videoEncoder = null
            OpenGLJniLib.releaseVideoSurface()
            isReady.set(false)
        }
        audioEncoder?.stop()
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

    private fun getVideoFileAddress(time:Long): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+time+".mp4"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+time+".mp4"
        }
    }

    private fun getAudioFileAddress(time:Long): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+time+".aac"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+time+".pcm"
        }
    }
}