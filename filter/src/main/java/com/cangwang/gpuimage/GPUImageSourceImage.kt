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

package com.cangwang.gpuimage

import android.graphics.Bitmap

class GPUImageSourceImage : GPUImageSource {

    var bitmap: Bitmap? = null

    constructor(bitmap: Bitmap) {
        if (mNativeClassID != 0L) return
        GPUImage.runOnDraw(Runnable { mNativeClassID = GPUImage.nativeSourceImageNew() })
        setImage(bitmap)
    }

    fun setImage(bitmap: Bitmap) {
        this.bitmap = bitmap
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L)
                GPUImage.nativeSourceImageSetImage(mNativeClassID, bitmap)
        })
    }

    @JvmOverloads
    fun destroy(onGLThread: Boolean = true) {
        if (mNativeClassID != 0L) {
            if (onGLThread) {
                GPUImage.runOnDraw(Runnable {
                    if (mNativeClassID != 0L) {
                        GPUImage.nativeSourceImageDestroy(mNativeClassID)
                        mNativeClassID = 0
                    }
                })
            } else {
                GPUImage.nativeSourceImageDestroy(mNativeClassID)
                mNativeClassID = 0
            }
        }
    }
}
