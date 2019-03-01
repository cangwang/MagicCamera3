package com.cangwang.magic.video


import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.media.MediaMuxer
import android.opengl.EGL14
import android.util.Log
import android.view.Surface
import java.io.File
import java.lang.RuntimeException

class VideoEncoderCoder(width: Int, height: Int, bitRate: Int, outFile: File) {
    companion object {
        const val TAG = "VideoEncoderCoder"
        const val MINE_TYPE = "video/avc"
        const val FRAME_RATE = 30
        const val IFRAME_INTERVAL = 5
    }

    private var mInputSurface:Surface
    private var mMuxer:MediaMuxer
    private var mEncoder:MediaCodec
    private var mBufferInfo:MediaCodec.BufferInfo = MediaCodec.BufferInfo()
    private var mTrackIndex = -1
    private var mMuxerStarted = false

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

        if (endOfStream) {
            Log.d(TAG, "sending EOS to encoder")
            mEncoder.signalEndOfInputStream()
        }

        var encoderOutputBuffers = mEncoder.outputBuffers
        while (true) {
            val encoderStatus = mEncoder.dequeueOutputBuffer(mBufferInfo, TIMEOUT_USEC)
            Log.d(TAG, "drainEncoder($endOfStream),endcoderStatus($encoderStatus) ,eglCurrentContext(${EGL14.eglGetCurrentContext()})")
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
                // should happen before receiving buffers, and should only happen once
                if (mMuxerStarted) {
                    throw RuntimeException("format changed twice")
                }
                val newFormat = mEncoder.outputFormat
                Log.d(TAG, "encoder output format changed: $newFormat")

                // now that we have the Magic Goodies, start the muxer
                mTrackIndex = mMuxer.addTrack(newFormat)
                mMuxer.start()
                mMuxerStarted = true
            } else if (encoderStatus < 0) {
                Log.w(TAG, "unexpected result from encoder.dequeueOutputBuffer: $encoderStatus")
                // let's ignore it
            } else {
                val encodedData = encoderOutputBuffers[encoderStatus]
                        ?: throw RuntimeException("encoderOutputBuffer " + encoderStatus +
                                " was null")

                if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_CODEC_CONFIG != 0) {
                    // The codec config data was pulled out and fed to the muxer when we got
                    // the INFO_OUTPUT_FORMAT_CHANGED status.  Ignore it.
                    Log.d(TAG, "ignoring BUFFER_FLAG_CODEC_CONFIG")
                    mBufferInfo.size = 0
                }

                if (mBufferInfo.size != 0) {
                    if (!mMuxerStarted) {
                        throw RuntimeException("muxer hasn't started")
                    }

                    // adjust the ByteBuffer values to match BufferInfo (not needed?)
                    encodedData.position(mBufferInfo.offset)
                    encodedData.limit(mBufferInfo.offset + mBufferInfo.size)

                    mMuxer.writeSampleData(mTrackIndex, encodedData, mBufferInfo)
                    Log.d(TAG, "sent " + mBufferInfo.size + " bytes to muxer, ts=" +
                                mBufferInfo.presentationTimeUs)
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