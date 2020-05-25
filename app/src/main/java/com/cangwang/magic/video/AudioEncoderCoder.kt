package com.cangwang.magic.video

import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.util.Log
import java.io.FileNotFoundException
import java.io.IOException
import java.nio.ByteBuffer

/**
 * Created by cangwang on 2019.3.6
 * 录制完整mp4短视频音轨
 */
class AudioEncoderCoder(muxer:MediaMuxerCoder?){
    private val TAG = AudioEncoderCoder::class.java.simpleName
    private var audioEncoder:MediaCodec?=null
    private val SAMPLE_RATE = 44100
    private val mBufferInfo = MediaCodec.BufferInfo()
    private var mMuxer:MediaMuxerCoder?=null

    init {
        audioEncoder = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_AUDIO_AAC)
        val format = MediaFormat.createAudioFormat(MediaFormat.MIMETYPE_AUDIO_AAC,SAMPLE_RATE,2)
        format.setInteger(MediaFormat.KEY_BIT_RATE, 96000)
        format.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, DEFAULT_BUFFER_SIZE)
        format.setInteger(MediaFormat.KEY_AAC_PROFILE,MediaCodecInfo.CodecProfileLevel.AACObjectLC)
        audioEncoder?.configure(format,null,null,MediaCodec.CONFIGURE_FLAG_ENCODE)
        mMuxer=muxer
    }

    fun start(){
        audioEncoder?.start()
    }

    fun release(){
        audioEncoder?.stop()
        audioEncoder?.release()
        audioEncoder = null
    }

    var prevPresentationTimes = mBufferInfo.presentationTimeUs
    private fun getPTSUs(): Long {
        var result = System.nanoTime() / 1000
        if (result < prevPresentationTimes) {
            result += prevPresentationTimes - result
        }
        return result
    }

    var encoderOutputBuffers:Array<ByteBuffer>?=null
    fun encodePCMToAAC(bytes:ByteArray){
        try {
            audioEncoder?.apply {
                val inputBufferIndex = dequeueInputBuffer(0)
                if (inputBufferIndex > 0){
                    val inputBuffer = getInputBuffer(inputBufferIndex)
                    inputBuffer.clear()
                    inputBuffer.put(bytes)
                    audioEncoder?.queueInputBuffer(inputBufferIndex,0,bytes.size,getPTSUs(),0)
                }

                while (true) {
                    val encoderStatus = dequeueOutputBuffer(mBufferInfo, 0)

                    if (encoderStatus == MediaCodec.INFO_TRY_AGAIN_LATER) {
                        Log.d(TAG, "no output available, spinning to await EOS")
                        break
                    } else if (encoderStatus == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                        // not expected for an encoder
                        encoderOutputBuffers = outputBuffers
                    } else if (encoderStatus == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                        //一定要在这个节点上获取fomart设置到混合器的音轨上
                        // should happen before receiving buffers, and should only happen once
                        if (mMuxer?.hasAudioTrack() == true) {
                            Log.d(TAG,"audio format changed twice")
                            break
                        }
                        val newFormat = outputFormat
                        Log.d(TAG, "encoder output format changed: $newFormat")

                        // now that we have the Magic Goodies, start the muxer
                        val track = mMuxer?.setAudioTrack(newFormat)
                        Log.d(TAG, "audiotrack: $track")
                        mMuxer?.start()
                    } else if (encoderStatus < 0) {
                        Log.w(TAG, "unexpected result from encoder.dequeueOutputBuffer: $encoderStatus")
                        // let's ignore it
                    } else {
                        encoderOutputBuffers = outputBuffers
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
                                    //释放采集的音轨数据
                                    releaseOutputBuffer(encoderStatus, false)
                                    break
//                                    throw RuntimeException("muxer hasn't started")
                                }

                                val outputBufferSize = encodedData.limit() + 7 // ADTS头部是7个字节
                                val aacBytes = ByteArray(outputBufferSize)
                                addADTStoPacket(aacBytes,outputBufferSize)
                                encodedData.get(aacBytes,7,encodedData.limit())
                                mBufferInfo.presentationTimeUs = getPTSUs()
                                mMuxer?.writeAudioData(encodedData,mBufferInfo)

//                                Log.d(TAG, "sent " + mBufferInfo.size + " bytes to muxer, ts=" + mBufferInfo.presentationTimeUs)
                            }
                            //释放采集的音轨数据
                            releaseOutputBuffer(encoderStatus, false)

                            if (mBufferInfo.flags and MediaCodec.BUFFER_FLAG_END_OF_STREAM != 0) {
                                Log.d(TAG, "end of stream reached")
                                break      // out of while
                            }
                        }
                    }
                }
            }
        }catch (e:FileNotFoundException){
            Log.e(TAG,e.toString())
        }catch (e:IOException){
            Log.e(TAG,e.toString())
        }
    }

    private fun addADTStoPacket(packet: ByteArray, packetLen: Int) {
        val profile = 2  //AAC LC，MediaCodecInfo.CodecProfileLevel.AACObjectLC;
        val freqIdx = 4  //见后面注释avpriv_mpeg4audio_sample_rates中32000对应的数组下标，来自ffmpeg源码
        val chanCfg = 1  //见后面注释channel_configuration，AudioFormat.CHANNEL_IN_MONO 单声道(声道数量)

        /*int avpriv_mpeg4audio_sample_rates[] = {96000, 88200, 64000, 48000, 44100, 32000,24000, 22050, 16000, 12000, 11025, 8000, 7350};
        channel_configuration: 表示声道数chanCfg
        0: Defined in AOT Specifc Config
        1: 1 channel: front-center
        2: 2 channels: front-left, front-right
        3: 3 channels: front-center, front-left, front-right
        4: 4 channels: front-center, front-left, front-right, back-center
        5: 5 channels: front-center, front-left, front-right, back-left, back-right
        6: 6 channels: front-center, front-left, front-right, back-left, back-right, LFE-channel
        7: 8 channels: front-center, front-left, front-right, side-left, side-right, back-left, back-right, LFE-channel
        8-15: Reserved
        */

        // fill in ADTS data
        packet[0] = 0xFF.toByte()
        //packet[1] = (byte)0xF9;
        packet[1] = 0xF1.toByte()//解决ios 不能播放问题
        packet[2] = ((profile - 1).shl(6) + (freqIdx shl 2)+(chanCfg shr 2)).toByte()
        packet[3] = ((chanCfg and 3 shl 6) + (packetLen shr 11)).toByte()
        packet[4] = (packetLen and 0x7FF shr 3).toByte()
        packet[5] = ((packetLen and 7 shl 5) + 0x1F).toByte()
        packet[6] = 0xFC.toByte()
    }

}