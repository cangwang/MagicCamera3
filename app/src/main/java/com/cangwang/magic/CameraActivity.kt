package com.cangwang.magic

import android.Manifest
import android.content.pm.ActivityInfo
import android.content.pm.PackageManager
import android.graphics.Point
import android.hardware.Camera
import android.os.Bundle

import android.view.*
import android.view.animation.Animation
import android.view.animation.Transformation
import android.widget.FrameLayout
import android.widget.RelativeLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.PermissionChecker
import com.cangwang.magic.camera.CameraTouch
import com.cangwang.magic.util.CameraHelper
import com.cangwang.magic.view.CameraSurfaceCallback
import com.werb.pickphotoview.util.PickUtils
import kotlinx.android.synthetic.main.activity_camera.*

/**
 * Created by cangwang on 2018/9/12.
 */
class CameraActivity: AppCompatActivity(){
    private val MODE_PIC = 1
    private var CAMERA_PERMISSION_REQ = 1

    var mCamera: Camera?=null
    private val ASPECT_RATIO_ARRAY = floatArrayOf(9.0f / 16, 3.0f / 4)
    var mAspectRatio = ASPECT_RATIO_ARRAY[0]

    var mCameraId = Camera.CameraInfo.CAMERA_FACING_BACK
    var surfaceCallback:CameraSurfaceCallback?=null
    var mCameraTouch:CameraTouch?=null

    private var mFoucesAnimation: FoucesAnimation? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN)
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        setContentView(R.layout.activity_camera)
        if (PermissionChecker.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED) run {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.CAMERA),CAMERA_PERMISSION_REQ)
        }else {
            initView()
        }
    }

    private fun initView(){
        btn_camera_filter.visibility = View.GONE
        btn_camera_shutter.visibility = View.GONE

        btn_camera_switch.setOnClickListener {

        }

        btn_camera_mode.visibility = View.GONE
        btn_camera_beauty.visibility = View.GONE

        val screenSize =Point()
        windowManager.defaultDisplay.getSize(screenSize)
        val params = glsurfaceview_camera.layoutParams as RelativeLayout.LayoutParams
        params.width= screenSize.x;
        params.height = screenSize.x* 16/9
        glsurfaceview_camera.layoutParams = params
        glsurfaceview_camera.setOnTouchListener(object :View.OnTouchListener{
            var mClickOn = 0L
            var mLastX = 0f
            var mLastY = 0f
            override fun onTouch(v: View?, event: MotionEvent?): Boolean {
                when(event?.actionMasked){
                    MotionEvent.ACTION_DOWN ->{
                        if (event.pointerCount == 1){
                            mClickOn = System.currentTimeMillis()
                            mLastX = event.x
                            mLastY = event.y
                        }
                    }
                    MotionEvent.ACTION_UP ->{
                        if (event.pointerCount == 1 && System.currentTimeMillis()- mClickOn <500){
                            moveFocus(event.x.toInt(), event.y.toInt())
                        }
                    }
                    MotionEvent.ACTION_POINTER_DOWN ->{
                        mCameraTouch?.onScaleStart(event){

                        }
                        return true
                    }
                    MotionEvent.ACTION_MOVE ->{
                        if (event.pointerCount == 2){
                            mCameraTouch?.onScale(event)
                            return true
                        }
                        else{
                            val x = event.x - mLastX
                            val y = event.y - mLastY
                            if (Math.abs(x) >=10 || Math.abs(y) >=10){
                                mClickOn = 0L
                            }
                        }
                    }
                    MotionEvent.ACTION_POINTER_UP ->{
                        mCameraTouch?.onScaleEnd {

                        }
                        return true
                    }
                }
                return false
            }

        })
    }

    override fun onResume() {
        super.onResume()
        mCamera = openCamera(glsurfaceview_camera.holder)
        surfaceCallback = CameraSurfaceCallback(mCamera)
        glsurfaceview_camera.holder.addCallback(surfaceCallback)
    }

    override fun onPause() {
        super.onPause()
        mCamera?.stopPreview()
        mCamera?.release()
        glsurfaceview_camera.holder.removeCallback(surfaceCallback)
        surfaceCallback?.releaseOpenGL()
        surfaceCallback=null
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        if (requestCode == CAMERA_PERMISSION_REQ &&(grantResults.size != 1 || grantResults[0] == PackageManager.PERMISSION_GRANTED)) {
            initView()
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    private fun openCamera(holder: SurfaceHolder?):Camera?{
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            return null
        }

        if (mCamera!=null){
            return mCamera
        }
        mCamera = CameraHelper.openCamera(mCameraId)

        mCamera?.let {
            //这里android 相机长和宽默认偏移90度，所以传入要对调
            CameraHelper.setOptimalSize(it,mAspectRatio,CameraHelper.getScreenHeight(),CameraHelper.getScreenWidth())
            CameraHelper.setDisplayOritation(this,it,mCameraId)
        }
        return mCamera
    }

    fun moveFocus(x:Int,y:Int){
        video_fouces.apply {
            visibility = View.VISIBLE
            val lp = video_fouces.layoutParams as RelativeLayout.LayoutParams
            layoutParams = lp
            mFoucesAnimation?.duration = 500
            mFoucesAnimation?.repeatCount = 0
            mFoucesAnimation?.setOldMargin(x,y)
            video_fouces.startAnimation(mFoucesAnimation)
        }
    }

    private fun removeImageFoucesRunnable() {
        video_fouces.removeCallbacks(mImageFoucesRunnable)
    }

    private fun imageFoucesDelayedHind() {
        video_fouces.postDelayed(mImageFoucesRunnable, 1000)
    }

    private val mImageFoucesRunnable = Runnable { video_fouces.visibility = View.GONE }

    private inner class FoucesAnimation : Animation() {

        private val width = PickUtils.dp2px(this@CameraActivity, 65f)
        private val W = PickUtils.dp2px(this@CameraActivity, 65f)

        private var oldMarginLeft: Int = 0
        private var oldMarginTop: Int = 0

        override fun applyTransformation(interpolatedTime: Float, t: Transformation) {

            val layoutParams = video_fouces.layoutParams as FrameLayout.LayoutParams
            var w = (width * (1 - interpolatedTime)).toInt()
            if (w < W) {
                w = W
            }
            layoutParams.width = w
            layoutParams.height = w
            if (w == W) {
                video_fouces.layoutParams = layoutParams
                return
            }
            layoutParams.leftMargin = oldMarginLeft - w / 2
            layoutParams.topMargin = oldMarginTop + w / 8
            video_fouces.layoutParams = layoutParams
        }

        fun setOldMargin(oldMarginLeft: Int, oldMarginTop: Int) {
            this.oldMarginLeft = oldMarginLeft
            this.oldMarginTop = oldMarginTop
            removeImageFoucesRunnable()
            imageFoucesDelayedHind()
        }
    }
}