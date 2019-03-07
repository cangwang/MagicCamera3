package com.cangwang.magic.camera

import android.util.Log
import android.view.MotionEvent

class CameraTouch(cameraCompat:CameraCompat) {
    var mOldScale = 1.0f
    var mScale = 0f
    var mSpan = 0f
    var mOldSpan = 0f
    var mFirstDistance = 0f
    var cameraCompat:CameraCompat?=null

    init {
        this.cameraCompat = cameraCompat
    }

    fun onScale(event:MotionEvent){
        if (event.pointerCount == 2){

            val distance = distance(event)
            if (mFirstDistance == 0f){
                mFirstDistance = distance
            }
            var scale = 0f
            when {
                distance > mFirstDistance -> {
                    scale = (distance - mFirstDistance) / 80
                    scale += mSpan
                    mOldSpan = scale
                    mScale = scale
                }
                distance < mFirstDistance -> {
                    scale = distance/mFirstDistance
                    mOldSpan = scale
                    mScale = scale.times(mOldScale)
                }
                else -> return
            }


        }
    }

    fun onScaleStart(event: MotionEvent,callback:(maxZoom:Float)->Unit){
        mFirstDistance = 0f
        callback(cameraCompat?.getMaxZoom()?:0f)
    }

    fun onScaleEnd(callback: ((maxZoom: Float) -> Unit)?){
        mOldScale = when {
            mScale < 1.0f -> 1.0f
            mScale > cameraCompat?.getMaxZoom()?:0f -> cameraCompat?.getMaxZoom()?:0f
            else -> mScale
        }
        mSpan = mOldSpan
        callback?.invoke(mOldScale)
        Log.e("scale", "scale:end")
    }

    fun resetScale(){
        mOldScale = 1.0f
        mSpan = 0f
        mFirstDistance = 0f
    }

    fun setScale(scale:Float){
        mScale = scale
        mOldScale = scale
        onScaleEnd(null)
    }

    private fun distance(event:MotionEvent):Float{
        val dx = event.getX(1) - event.getX(0)
        val dy = event.getY(1) - event.getY(0)
        return Math.sqrt(dx.times(dx).toDouble() + dy.times(dy).toDouble()).toFloat()
    }
}