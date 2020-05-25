package com.cangwang.magic.video

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.util.Log
import android.view.Surface
import java.nio.ByteBuffer

/**
 * Created by cangwang on 2019.3.6
 * 录制完整mp4短视频视轨
 */
class VideoEncoderCoder(width: Int, height: Int, bitRate: Int, muxer:MediaMuxerCoder?) {
    companion object {
        const val TAG = "VideoEncoderCoder"
        const val MINE_TYPE = "video/avc"
        const val FRAME_RATE = 30
        const val IFRAME_INTERVAL = 5
    }

    private var mInputSurface:Surface
    private var mMuxer:MediaMuxerCoder?=null
    private var mEncoder:MediaCodec
    private var mBufferInfo:MediaCodec.BufferInfo = MediaCodec.BufferInfo()

    var encoderOutputBuffers:Array<ByteBuffer>?=null

    init {
        val format = MediaFormat.createVideoFormat(MINE_TYPE,width,height)
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT,MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface)
        format.setInteger(MediaFormat.KEY_BIT_RATE,bitRate)
        format.setInteger(MediaFormat.KEY_FRAME_RATE, FRAME_RATE)
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, IFRAME_INTERVAL)
        Log.d(TAG, "format: $format")
        mEncoder = MediaCodec.createEncoderByType(MINE_TYPE)
        mEncoder.configure(format,null,null,MediaCodec.CONFIGURE_FLAG_ENCODE)
        mInputSurface = mEncoder.createInputSurface()

        mMuxer = muxer
//        val track = mMuxer?.setVideoTrack(format)
//        Log.d(TAG, "videotrack: $track")
    }

    fun start(){
        mEncoder.start()
    }

    fun stop(){
        mEncoder.stop()
    }

    fun getInputSurface():Surface{
        return mInputSurface
    }

    fun release(){
        Log.d(TAG,"release encoder objects")
        mEncoder.stop()
        mEncoder.release()
    }

    /**
     * 视频录制输入
     * endOfStream 加入采集结束符
     */
    fun drainEncoder(endOfStream:Boolean){
        val TIMEOUT_USEC = 10000L

        if (endOfStream) {
            Log.d(TAG, "sending EOS to encoder")
            //只有视频流需要关闭输入标志
            mEncoder.signalEndOfInputStream()
        }

        while (true) {
            val encoderStatus = mEncoder.dequeueOutputBuffer(mBufferInfo, TIMEOUT_USEC)
//            Log.d(TAG, "drainEncoder($endOfStream),endcoderStatus($encoderStatus)")
            if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                // no output available yet
                if (!endOfStream) {
                    break      // out of while
                } else {
                    Log.d(TAG, "no output available, spinning to await EOS")
                }
            } else if (encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                // not expected for an encoder
                encoderOutputBuffers = mEncoder.outputBuffers
            } else if (encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                //一定要在这个节点上获取fomart设置到混合器的视轨上
                // should happen before receiving buffers, and should only happen once
                if (mMuxer?.hasVideoTrack() == true) {
                    Log.d(TAG,"video format changed twice")
                    return
                }
                val newFormat = mEncoder.outputFormat
                Log.d(TAG, "encoder output format changed: $newFormat")

                // now that we have the Magic Goodies, start the muxer
                val track = mMuxer?.setVideoTrack(newFormat)
                Log.d(TAG, "videotrack: $track")
                mMuxer?.start()
            } else if (encoderStatus < 0) {
                Log.w(TAG, "unexpected result from encoder.dequeueOutputBuffer: $encoderStatus")
                // let's ignore it
            } else {
                encoderOutputBuffers = mEncoder.outputBuffers
                if (encoderOutputBuffers != null) {
                    val encodedData = encoderOutputBuffers!![encoderStatus]

                    if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_CODEC_CONFIG != 0) {
                        // The codec config data was pulled out and fed to the muxer when we got
                        // the INFO_OUTPUT_FORMAT_CHANGED status.  Ignore it.
                        Log.d(TAG, "ignoring BUFFER_FLAG_CODEC_CONFIG")
                        mBufferInfo.size = 0
                    }

                    if (mBufferInfo.size != 0) {
                        if (mMuxer?.isRecording() == false) {
                            Log.d(TAG,"muxer hasn't started")
                            break
//                            throw RuntimeException("muxer hasn't started")
                        }

                        // adjust the ByteBuffer values to match BufferInfo (not needed?)
                        encodedData.position(mBufferInfo.offset)
                        encodedData.limit(mBufferInfo.offset + mBufferInfo.size)

                        mMuxer?.writeVideoData(encodedData, mBufferInfo)
//                        Log.d(TAG, "sent " + mBufferInfo.size + " bytes to muxer, ts=" + mBufferInfo.presentationTimeUs)
                    }

                    mEncoder.releaseOutputBuffer(encoderStatus, false)

                    if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM != 0) {
                        if (!endOfStream) {
                            Log.w(TAG, "reached end of stream unexpectedly")
                        } else {
                            Log.d(TAG, "end of stream reached")
                        }
                        break      // out of while
                    }
                }
            }
        }
    }
}