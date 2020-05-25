package com.cangwang.magic.video

import android.media.MediaCodec
import android.media.MediaFormat
import android.media.MediaMuxer
import android.util.Log
import java.io.File
import java.nio.ByteBuffer
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by cangwang on 2019.3.6
 * 录制完整mp4小视频混合器
 */
class MediaMuxerCoder(outFile: File){
    private val TAG= MediaMuxerCoder::class.java.simpleName
    private var mMuxer: MediaMuxer?=null
    private var NONE_TRACK = -1
    private var videoTrack = NONE_TRACK
    private var audioTrack = NONE_TRACK
    private var mMuxerStarted= AtomicBoolean(false)

    init {
        mMuxer = MediaMuxer(outFile.toString(),MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
    }

    /**
     * 设置视轨
     */
    fun setVideoTrack(mediaFormat: MediaFormat):Int{
        videoTrack = mMuxer?.addTrack(mediaFormat)?:-1
        return videoTrack
    }

    /**
     * 设置音轨
     */
    fun setAudioTrack(mediaFormat: MediaFormat):Int{
        audioTrack = mMuxer?.addTrack(mediaFormat)?:-1
        return audioTrack
    }

    fun reset(outFile: File){
        release()
        mMuxer = MediaMuxer(outFile.toString(),MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
        videoTrack = NONE_TRACK
        audioTrack = NONE_TRACK
        mMuxerStarted.set(false)
    }

    /**
     * 开启混合器
     */
    fun start(){
        //等待音轨和视轨同时设置完成后开启混合器
        if (videoTrack != NONE_TRACK && audioTrack != NONE_TRACK) {
            mMuxer?.start()
            mMuxerStarted.set(true)
            Log.d(TAG,"muxer start")
        }
    }

    /**
     * 是否已经写入音轨
     */
    fun hasAudioTrack():Boolean{
        return audioTrack != NONE_TRACK
    }

    /**
     * 是否已经写入视轨
     */
    fun hasVideoTrack():Boolean{
        return videoTrack != NONE_TRACK
    }

    fun isRecording():Boolean{
        return mMuxerStarted.get()
    }

    /**
     * 写入视频数据
     */
    fun writeVideoData(byteBuffer: ByteBuffer,bufferInfo: MediaCodec.BufferInfo){
        if (videoTrack != NONE_TRACK && mMuxerStarted.get()) {
            writeData(videoTrack, byteBuffer, bufferInfo)
        }
    }

    /**
     * 写入音频数据
     */
    fun writeAudioData(byteBuffer: ByteBuffer,bufferInfo: MediaCodec.BufferInfo){
        if (audioTrack != NONE_TRACK && mMuxerStarted.get()) {
            writeData(audioTrack, byteBuffer, bufferInfo)
        }
    }

    /**
     * 写入数据到混合器
     */
    private fun writeData(track:Int, byteBuffer: ByteBuffer, bufferInfo: MediaCodec.BufferInfo){
        synchronized(MediaMuxerCoder::class.java){
            Log.d(TAG, "track = $track, sent " + bufferInfo.size + " bytes to muxer, ts=" + bufferInfo.presentationTimeUs)
            mMuxer?.writeSampleData(track, byteBuffer, bufferInfo)
        }
    }

    /**
     * 释放混合器资源
     */
    fun release(){
        mMuxer?.stop()
        mMuxer?.release()
        mMuxer=null
    }
}