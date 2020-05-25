/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cangwang.magic

import android.app.Activity
import android.graphics.Bitmap
import android.media.MediaScannerConnection
import android.os.Bundle
import android.os.Environment
import android.view.View
import android.widget.SeekBar
import android.widget.Toast

import com.cangwang.gpuimage.GPUImage
import com.cangwang.gpuimage.GPUImageFilter
import com.cangwang.gpuimage.GPUImageSource
import com.cangwang.gpuimage.GPUImageSourceCamera
import com.cangwang.gpuimage.GPUImageView
import com.cangwang.magic.util.FilterHelper
import kotlinx.android.synthetic.main.activity_camera_sample.*

import java.io.File
import java.io.FileNotFoundException
import java.io.FileOutputStream

class CameraSampleActivity : Activity(), View.OnClickListener, SeekBar.OnSeekBarChangeListener,
        FilterHelper.OnFilterSelectedListener {

    private var sourceCamera: GPUImageSourceCamera? = null
    private var filter: GPUImageFilter? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera_sample)

        btn_flip_cam.setOnClickListener(this)
        btn_filter.setOnClickListener(this)
        btn_capture.setOnClickListener(this)

        val seekBar = findViewById<View>(R.id.seek) as SeekBar
        seekBar.setOnSeekBarChangeListener(this)
        seekBar.progress = 50

        sourceCamera = GPUImageSourceCamera(this@CameraSampleActivity)
        filter = GPUImageFilter.create("BrightnessFilter")
        filter?.let {
            sourceCamera?.addTarget(it)
                    ?.addTarget(findViewById<View>(R.id.gpuimagexview) as GPUImageView)
        }
        sourceCamera?.let {
            GPUImage.setSource(it)
        }
    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.btn_flip_cam -> if (sourceCamera != null) {
                sourceCamera!!.switchCamera()
            }
            R.id.btn_filter -> FilterHelper.showListDialog(this, this)
            R.id.btn_capture -> sourceCamera?.captureAProcessedFrameData(filter,
                    object : GPUImageSource.ProcessedFrameDataCallback {
                        override fun onResult(result: Bitmap?) {
                            if (result != null) {
                                val path = Environment
                                        .getExternalStoragePublicDirectory(
                                                Environment.DIRECTORY_PICTURES)
                                val file = File(path,
                                        "gpuimage-x" + "/" + System.currentTimeMillis() + ".jpg")
                                try {
                                    file.parentFile.mkdirs()
                                    result.compress(Bitmap.CompressFormat.JPEG, 80,
                                            FileOutputStream(file))

                                    // make a toast
                                    runOnUiThread {
                                        Toast.makeText(this@CameraSampleActivity,
                                                "Image Saved:" + file.absolutePath,
                                                Toast.LENGTH_SHORT).show()
                                    }

                                    // Tell the media scanner about the new file,
                                    // so that it is immediately present in your album.
                                    MediaScannerConnection.scanFile(this@CameraSampleActivity,
                                            arrayOf(file.toString()), null, null)
                                } catch (e: FileNotFoundException) {
                                    e.printStackTrace()
                                }
                            }
                        }
                    })
            else -> {
            }
        }
    }

    override fun OnFilterSelected(newFilter: GPUImageFilter) {
        filter?.let {
            sourceCamera?.removeTarget(it)
            it.destroy() // destroy instance if you want
        }
        sourceCamera?.addTarget(newFilter)
                ?.addTarget(findViewById<View>(R.id.gpuimagexview) as GPUImageView)
        filter = newFilter
        //sourceCamera.proceed();
    }

    override fun onProgressChanged(seekBar: SeekBar, progress: Int, fromUser: Boolean) {
        if (sourceCamera != null && filter != null) {
            val value = progress / 100.0f // let the value between 0 and 1
            FilterHelper.applyFilterWithSliderValue(filter!!, value)
            //sourceCamera.proceed();
        }
    }

    override fun onResume() {
        super.onResume()
        if (sourceCamera != null) {
            sourceCamera!!.onResume()
        }
    }

    override fun onPause() {
        if (sourceCamera != null) {
            sourceCamera!!.onPause()
        }
        super.onPause()
    }

    override fun onDestroy() {
        GPUImage.destroy()
        super.onDestroy()
    }

    override fun onStartTrackingTouch(seekBar: SeekBar) {}

    override fun onStopTrackingTouch(seekBar: SeekBar) {}
}
