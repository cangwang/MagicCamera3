package com.cangwang.gpuimage

import android.graphics.Bitmap
import java.nio.ByteBuffer

abstract class GPUImageSource {
    var mNativeClassID = 0L

    fun addTarget(target: GPUImageTarget): GPUImageSource? {
        return addTarget(target, -1)
    }

    fun addTarget(target: GPUImageTarget, textID: Int): GPUImageSource? {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeSourceAddTarget(mNativeClassID, target.getNativeClassID(), textID,
                        target is GPUImageFilter)
            }
        })

        return target as? GPUImageSource
    }

    fun removeTarget(target: GPUImageTarget) {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L && target.getNativeClassID() != 0L) {
                GPUImage.nativeSourceRemoveTarget(mNativeClassID, target.getNativeClassID(),
                        target is GPUImageFilter)
            }
        })
    }

    fun removeAllTargets() {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeSourceRemoveAllTargets(mNativeClassID)
            }
        })
    }

    fun proceed() {
        proceed(true, true)
    }

    fun proceed(bUpdateTargets: Boolean, bRequestRender: Boolean) {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeSourceProceed(mNativeClassID, bUpdateTargets)
            }
        })
        if (bRequestRender) {
            GPUImage.requestRender()
        }
    }

    fun getRotatedFramebufferWidth(): Int {
        return GPUImage.nativeSourceGetRotatedFramebuferWidth(mNativeClassID)
    }

    fun getRotatedFramebufferHeight(): Int {
        return GPUImage.nativeSourceGetRotatedFramebuferHeight(mNativeClassID)
    }

    fun captureAProcessedFrameData(
            upToFilter: GPUImageFilter?, proceedResult: ProcessedFrameDataCallback
    ) {
        captureAProcessedFrameData(upToFilter, getRotatedFramebufferWidth(),
                getRotatedFramebufferHeight(), proceedResult)
    }

    fun captureAProcessedFrameData(
            upToFilter: GPUImageFilter?, width: Int, height: Int,
            proceedResult: ProcessedFrameDataCallback
    ) {
        if (upToFilter == null) {
            return
        }
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                val resultData = GPUImage.nativeSourceCaptureAProcessedFrameData(mNativeClassID,
                        upToFilter.getNativeClassID(), width, height)
                if (resultData != null) {
                    val bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
                    bmp.copyPixelsFromBuffer(ByteBuffer.wrap(resultData))
                    proceedResult.onResult(bmp)
                }
            }
        })
        GPUImage.requestRender()
    }

    interface ProcessedFrameDataCallback {
        fun onResult(result: Bitmap?)
    }
}