package com.cangwang.gpuimage

class GPUImageFilter : GPUImageSource, GPUImageTarget {

    var filterClassName: String? = null

    companion object {
        fun create(filterName: String): GPUImageFilter {
            return GPUImageFilter(filterName)
        }
    }

    constructor(filterClassName: String) {
        if (mNativeClassID != 0L) {
            return
        }
        GPUImage.runOnDraw(Runnable {
            mNativeClassID = GPUImage.nativeFilterCreate(filterClassName)
        })
        this.filterClassName = filterClassName
    }

    fun setProperty(property: String, value: Double) {
        setProperty(property, value.toFloat())
    }

    fun setProperty(property: String, value: Float) {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeFilterSetPropertyFloat(mNativeClassID, property, value)
            }
        })
    }

    fun setProperty(property: String, value: Int) {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeFilterSetPropertyInt(mNativeClassID, property, value)
            }
        })
    }

    fun setProperty(property: String, value: String) {
        GPUImage.runOnDraw(Runnable {
            if (mNativeClassID != 0L) {
                GPUImage.nativeFilterSetPropertyString(mNativeClassID, property, value)
            }
        })
    }

    override fun getNativeClassID(): Long {
        return mNativeClassID
    }

    fun destroy() {
        destroy(true)
    }

    fun destroy(onGLThread: Boolean) {
        if (mNativeClassID != 0L) {
            if (onGLThread) {
                GPUImage.runOnDraw(Runnable {
                    if (mNativeClassID != 0L) {
                        GPUImage.nativeFilterDestroy(mNativeClassID);
                        mNativeClassID = 0L
                    }
                })
            } else {
                GPUImage.nativeFilterDestroy(mNativeClassID)
                mNativeClassID = 0L
            }
        }
    }
}