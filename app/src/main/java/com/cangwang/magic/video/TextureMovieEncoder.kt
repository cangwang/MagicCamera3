package com.cangwang.magic.video

import android.annotation.SuppressLint
import android.os.Build
import android.os.Environment
import android.util.Log
import com.cangwang.magic.BaseApplication
import com.cangwang.magic.util.OpenGLJniLib
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.functions.BiFunction
import io.reactivex.schedulers.Schedulers
import java.io.File
import java.lang.Exception
import java.util.concurrent.Executors
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by cangwang on 2019.3.6
 * 录制完整mp4短视频
 */
class TextureMovieEncoder{
    private val TAG = "TextureMovieEncoder"
    private var videoEncoder:VideoEncoderCoder?=null
    private var videoThread = Executors.newSingleThreadExecutor()
    private var audioThread = Executors.newSingleThreadExecutor()
    public var isReady = AtomicBoolean(false)
    private var audioEncoder:AudioRecordCoder?=null
    private var muxerEncoder:MediaMuxerCoder?=null

    /**
     * 开始采集
     */
    @SuppressLint("CheckResult")
    fun startRecord(width:Int, height:Int, textureId:Int, filterType:Int){
        //混合器
        muxerEncoder = MediaMuxerCoder(File(getVideoFileAddress(System.currentTimeMillis())))
        //开启视频和音频采集
        Observable.zip(startVideo(width, height, textureId, filterType),startAudio(),
                object: BiFunction<Boolean, Boolean, Boolean>{
                    override fun apply(isVideoReady: Boolean, isAudioReady: Boolean): Boolean {
                        return isVideoReady && isAudioReady
                    }
                }).observeOn(Schedulers.io())
                .subscribe ({
                    isReady.set(true)
                },{
                    Log.e(TAG,"init error $it")
                })
    }

    /**
     * 视频采集
     */
    private fun startVideo(width:Int, height:Int, textureId:Int, filterType:Int):Observable<Boolean>{
        return Observable.create(ObservableOnSubscribe<Boolean> {
            if (width > 0 && height > 0) {
                //码率为4*高*宽，使用滤镜，太低码率无法记录足够的细节
                videoEncoder = VideoEncoderCoder(width, height, width*height*4, muxerEncoder)
                videoEncoder?.apply {
                    OpenGLJniLib.buildVideoSurface(getInputSurface(), textureId, BaseApplication.context.assets)
                    OpenGLJniLib.magicVideoFilterChange(width,height)
                    if (filterType >= 0) {
                        OpenGLJniLib.setVideoFilterType(filterType)
                    }
                    start()
                }
            }
            it.onNext(true)
        }).subscribeOn(Schedulers.from(videoThread))
    }

    /**
     * 音频采集
     */
    private fun startAudio():Observable<Boolean>{
        return Observable.create(ObservableOnSubscribe<Boolean> {
            audioEncoder = AudioRecordCoder()
            audioEncoder?.start(muxerEncoder)
            it.onNext(true)
        }).subscribeOn(Schedulers.from(audioThread))
    }

    /**
     * 停止录制
     */
    @SuppressLint("CheckResult")
    fun stopRecord(){
        Observable.zip(stopVideo(),stopAudio(),
                object: BiFunction<Boolean, Boolean, Boolean>{
                    override fun apply(isVideoStop: Boolean, isAudioStop: Boolean): Boolean {
                        return isVideoStop && isAudioStop
                    }
                }).observeOn(Schedulers.io())
                .subscribe ({
                    muxerEncoder?.release()
                    Log.d(TAG,"muxer release")
                },{
                    Log.e(TAG,it.toString())
                })
    }

    /**
     * 停止视频录制
     */
    private fun stopVideo():Observable<Boolean>{
        return Observable.create(ObservableOnSubscribe<Boolean> {
            isReady.set(false)
            videoEncoder?.drainEncoder(true)
            videoEncoder?.release()
            videoEncoder = null
            OpenGLJniLib.releaseVideoSurface()
            it.onNext(true)
        }).subscribeOn(Schedulers.from(videoThread))
    }

    /**
     * 停止音频录制
     */
    private fun stopAudio():Observable<Boolean>{
        return Observable.create(ObservableOnSubscribe<Boolean> {
            audioEncoder?.stop()
            it.onNext(true)
        }).subscribeOn(Schedulers.from(audioThread))
    }

    /**
     * 视频Surface绘制
     */
    fun drawFrame(matrix:FloatArray,time:Long){
        if(isReady.get()) {
            videoThread.execute {
                videoEncoder?.apply {
                    try {
                        //传输视频帧数据
                        drainEncoder(false)
                    }catch (e:Exception){
                        Log.e(TAG,e.toString())
                    }
                    //绘制Suface
                    OpenGLJniLib.magicVideoDraw(matrix, time)
                }
            }
        }
    }

    /**
     * 获取文件地址
     */
    private fun getVideoFileAddress(time:Long): String {
        return if(Build.BRAND == "Xiaomi"){ // 小米手机
            Environment.getExternalStorageDirectory().path +"/DCIM/Camera/"+time+".mp4"
        }else{  // Meizu 、Oppo
            Environment.getExternalStorageDirectory().path +"/DCIM/"+time+".mp4"
        }
    }
}