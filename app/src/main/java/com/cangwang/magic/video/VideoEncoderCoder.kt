package com.cangwang.magic.video

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.media.MediaMuxer
import android.util.Log
import android.view.Surface
import java.io.File
import java.lang.RuntimeException

class VideoEncoderCoder{
    companion object {
        const val TAG = "VideoEncoderCoder"
        const val MINE_TYPE = "video/avc"
        const val FRAME_RATE = 30
        const val IFRAME_INTERVAL = 5
    }

    private lateinit var mInputSurface:Surface
    private lateinit var mMuxer:MediaMuxer
    private lateinit var mEncoder:MediaCodec
    private var mBufferInfo:MediaCodec.BufferInfo = MediaCodec.BufferInfo()
    private var mTrackIndex = -1
    private var mMuxerStarted = false

    constructor(width:Int,height:Int,bitRate:Int,outFile:File){
        val format = MediaFormat.createVideoFormat(MINE_TYPE,width,height)
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT,MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface)
        format.setInteger(MediaFormat.KEY_BIT_RATE,bitRate)
        format.setInteger(MediaFormat.KEY_FRAME_RATE, FRAME_RATE)
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, IFRAME_INTERVAL)
        Log.d(TAG, "format: $format")

        mEncoder = MediaCodec.createEncoderByType(MINE_TYPE)
        mEncoder.configure(format,null,null,MediaCodec.CONFIGURE_FLAG_ENCODE)
        mInputSurface = mEncoder.createInputSurface()

        mMuxer = MediaMuxer(outFile.toString(),MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4)
        mTrackIndex = -1
        mMuxerStarted = false
    }

    fun start(){
        mEncoder.start()
    }

    fun stop(){
        mEncoder.stop()
    }

    fun resume(){

    }

    fun getInputSurface():Surface{
        return mInputSurface
    }

    fun release(){
        Log.d(TAG,"release encoder objects")
        mEncoder.stop()
        mEncoder.release()

        mMuxer.stop()
        mMuxer.release()
    }

    fun drainEncoder(endOfStream:Boolean){
        val TIMEOUT_USEC = 10000L
        Log.d(TAG,"drainEncoder($endOfStream)")
        if (endOfStream){
            Log.d(TAG,"sending EOS to encoder")
            mEncoder.signalEndOfInputStream()
        }

        var encoderOutputBuffers = mEncoder.outputBuffers
        while (true){
            val encoderStatus = mEncoder.dequeueOutputBuffer(mBufferInfo,TIMEOUT_USEC)
            if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER){
                if (!endOfStream){
                    break
                }else{
                    Log.d(TAG, "no output available, spinning to await EOS")
                }
            }else if(encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED){
                encoderOutputBuffers = mEncoder.outputBuffers
            }else if(encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED){
                if (mMuxerStarted){
                    throw RuntimeException("format changed twice")
                }
                val newFormat = mEncoder.outputFormat
                Log.d(TAG,"encoder output format changed: $newFormat")
                mTrackIndex = mMuxer.addTrack(newFormat)
                mMuxer.start()
                mMuxerStarted = true
            }else if (encoderStatus <0){
                Log.e(TAG, "unexpected result from encoder.dequeueOutputBuffer: $encoderStatus")
            }else{
                val encodedData = encoderOutputBuffers[encoderStatus] ?: throw RuntimeException(("encoderOutputBuffer " + encoderStatus +
                        " was null"))
                if ((mBufferInfo.flags and MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0){
                    Log.d(TAG, "ignoring BUFFER_FLAG_CODEC_CONFIG")
                    mBufferInfo.size = 0
                }

                if (mBufferInfo.size!=0){
                    if (!mMuxerStarted) {
                        throw RuntimeException("muxer hasn't started")
                    }
                    encodedData.position(mBufferInfo.offset)
                    encodedData.limit(mBufferInfo.offset+mBufferInfo.size)
                    mMuxer.writeSampleData(mTrackIndex,encodedData,mBufferInfo)
                    Log.d(TAG, "sent " + mBufferInfo.size + " bytes to muxer, ts=" +
                            mBufferInfo.presentationTimeUs)
                }
                mEncoder.releaseOutputBuffer(encoderStatus,false)

                if ((mBufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM)!=0){
                    if (!endOfStream){
                        Log.e(TAG, "reached end of stream unexpectedly")
                    }else{
                        Log.d(TAG, "end of stream reached")
                    }
                    break
                }
            }
        }
    }
}