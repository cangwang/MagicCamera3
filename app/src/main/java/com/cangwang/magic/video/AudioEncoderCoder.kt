package com.cangwang.magic.video


import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat

class AudioEncoderCoder{
    private lateinit var audioEncoder:MediaCodec
    private val AUDIO_MIME = "audio/mp4a-latm"
    init {
        audioEncoder = MediaCodec.createDecoderByType(AUDIO_MIME)
        val format = MediaFormat()
        format.setString(MediaFormat.KEY_MIME,AUDIO_MIME)
        format.setInteger(MediaFormat.KEY_BIT_RATE,32000)
        format.setInteger(MediaFormat.KEY_CHANNEL_COUNT,2)
        format.setInteger(MediaFormat.KEY_SAMPLE_RATE,48000)
        format.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, DEFAULT_BUFFER_SIZE)
        format.setInteger(MediaFormat.KEY_AAC_PROFILE,MediaCodecInfo.CodecProfileLevel.AACObjectLC)
        audioEncoder.configure(format,null,null,MediaCodec.CONFIGURE_FLAG_ENCODE)
    }

    fun start(){
        audioEncoder.start()
    }

    fun release(){
        audioEncoder.stop()
        audioEncoder.release()
    }


    fun encodePCMToAAC(bytes:ByteArray){

    }
}