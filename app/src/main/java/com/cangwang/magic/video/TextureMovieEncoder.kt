package com.cangwang.magic.video

import android.os.Build
import android.os.Environment
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.OpenGLJniLib
import java.io.File
import java.util.concurrent.Executors


class TextureMovieEncoder{
    private var videoEncoder:VideoEncoderCoder?=null
    private var recoredThread = Executors.newSingleThreadExecutor()

    fun startRecord(width:Int,height:Int,textureId:Int,filterType:Int){
        recoredThread.execute {
            if (width > 0 && height > 0) {
                videoEncoder = VideoEncoderCoder(width, height, 1000000, File(getVideoFileAddress()))
                videoEncoder?.apply {
                    OpenGLJniLib.buildVideoSurface(getInputSurface(), textureId, BaseApplication.context.assets)

                    if (filterType > 0) {
                        OpenGLJniLib.setVideoFilterType(filterType)
                    }
                    start()
                }
            }
        }
    }

    fun stopRecord(){
        recoredThread.execute {
            videoEncoder?.drainEncoder(true)
            videoEncoder?.release()
            videoEncoder = null
            OpenGLJniLib.releaseVideoSurface()
        }
    }

    fun drawFrame(matrix:FloatArray,time:Long){
        recoredThread.execute {
            videoEncoder?.apply {
                OpenGLJniLib.magicVideoDraw(matrix)
                drainEncoder(false)
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