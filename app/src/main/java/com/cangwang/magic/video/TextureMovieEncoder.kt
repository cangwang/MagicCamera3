package com.cangwang.magic.video

import android.os.Build
import android.os.Environment
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.OpenGLJniLib
import java.io.File
import java.util.concurrent.Executors


class TextureMovieEncoder{
    private var videoEncoder:VideoEncoderCoder?=null
    private var recordThread = Executors.newSingleThreadExecutor()

    fun startRecord(width:Int,height:Int,textureId:Int,filterType:Int){
        recordThread.execute {
            if (width > 0 && height > 0) {
                videoEncoder = VideoEncoderCoder(width, height, 1000000, File(getVideoFileAddress()))
                videoEncoder?.apply {
                    OpenGLJniLib.buildVideoSurface(getInputSurface(), textureId, BaseApplication.context.assets)
                    OpenGLJniLib.magicVideoFilterChange(width,height)
                    if (filterType > 0) {
                        OpenGLJniLib.setVideoFilterType(filterType)
                    }
                    start()
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
        }
    }

    fun drawFrame(matrix:FloatArray,time:Long){
        recordThread.execute {
            videoEncoder?.apply {
                drainEncoder(false)
                OpenGLJniLib.magicVideoDraw(matrix,time)
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