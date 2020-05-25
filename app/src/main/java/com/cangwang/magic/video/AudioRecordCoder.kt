package com.cangwang.magic.video

import android.media.AudioFormat
import android.media.AudioRecord
import android.media.MediaRecorder
import android.util.Log
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by cangwang on 2019.3.6
 * 录制完整mp4短视频音轨
 */
class AudioRecordCoder{
    val TAG = AudioRecordCoder::class.java.simpleName
    var SAMPLE_RATE = 44100
    val CHANNEL = AudioFormat.CHANNEL_CONFIGURATION_STEREO
    val AUDIO_FORMAT = AudioFormat.ENCODING_PCM_16BIT

    var audioRecorder:AudioRecord?=null
    var recordThread:Thread?=null
    var bufferSizeInBytes = 0
    var isRecoding = AtomicBoolean(false)
    var audioEncoder:AudioEncoderCoder?=null

    private var muxerCoder:MediaMuxerCoder?=null

    init {
        bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_RATE,CHANNEL,AUDIO_FORMAT)
        if (AudioRecord.ERROR_BAD_VALUE == bufferSizeInBytes || AudioRecord.ERROR == bufferSizeInBytes) {
            Log.e(TAG,"bufferSizeInBytes not init")
        }
        audioRecorder = AudioRecord(MediaRecorder.AudioSource.MIC,SAMPLE_RATE,CHANNEL,AUDIO_FORMAT,bufferSizeInBytes)

        if (audioRecorder == null || audioRecorder!!.state !=AudioRecord.STATE_INITIALIZED){
            SAMPLE_RATE = 16000
            bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLE_RATE,CHANNEL,AUDIO_FORMAT)
            audioRecorder = AudioRecord(MediaRecorder.AudioSource.MIC,SAMPLE_RATE,CHANNEL,AUDIO_FORMAT,bufferSizeInBytes)
        }
        if (audioRecorder!!.state !=AudioRecord.STATE_INITIALIZED){
            Log.e(TAG,"audio record not init")
        }

    }

    fun start(muxer: MediaMuxerCoder?){
        if(audioRecorder?.state == AudioRecord.STATE_INITIALIZED) {
            muxerCoder = muxer
            audioRecorder?.run {
                if (state != AudioRecord.STATE_INITIALIZED) {
                    null
                } else {
                    startRecording()
                }
            }
            isRecoding.set(true)
            audioEncoder = AudioEncoderCoder(muxerCoder)
            recordThread = Thread(RecordThread(), "AudioRecordThread")
            recordThread?.start()
            audioEncoder?.start()
        }
    }

    fun stop(){
        if(audioRecorder?.state == AudioRecord.STATE_INITIALIZED) {
            audioRecorder?.run {
                Log.d(TAG,"stop audio")
                isRecoding.set(false)
                recordThread?.join()
                Log.d(TAG,"release")
                audioEncoder?.release()
                releaseAudioRecord()
            }
        }
    }

    private fun releaseAudioRecord(){
        audioRecorder?.run {
            stop()
            release()
            Log.d(TAG,"audio stop record")
        }
    }

    fun getAudioRecordBuffer(bufferSizeInBytes:Int,audioSamples:ByteArray):Int{
        audioRecorder?.run {
            return read(audioSamples,0,bufferSizeInBytes)
        }?:return 0
    }

    inner class RecordThread:Runnable{
        override fun run() {
            val audioSamples = ByteArray(bufferSizeInBytes)
                while (isRecoding.get()){ //循环采集音轨
                    val audioSampleSize = getAudioRecordBuffer(bufferSizeInBytes,audioSamples)
                    if (audioSampleSize != 0){
                        audioEncoder?.encodePCMToAAC(audioSamples)
                    }
            }
        }
    }
}
