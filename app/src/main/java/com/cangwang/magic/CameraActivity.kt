package com.cangwang.magic

import android.content.pm.PackageManager
import android.graphics.Point
import android.hardware.Camera
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.SurfaceHolder
import android.widget.RelativeLayout
import com.cangwang.magic.util.CameraHelper
import com.cangwang.magic.view.CameraSurfaceCallback
import kotlinx.android.synthetic.main.activity_camera.*
import kotlinx.android.synthetic.main.filter_layout.*

/**
 * Created by cangwang on 2018/9/12.
 */
class CameraActivity:AppCompatActivity(){

    private var isRecording = false
    private val MODE_PIC = 1
    private val MODE_VIDEO = 2
    private var mode = MODE_PIC

    var mCamera: Camera?=null
    private val ASPECT_RATIO_ARRAY = floatArrayOf(9.0f / 16, 3.0f / 4)
    var mAspectRatio = ASPECT_RATIO_ARRAY[0]

    var mCameraId = Camera.CameraInfo.CAMERA_FACING_BACK

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera)
        initView()
    }

    fun initView(){
        btn_camera_filter.setOnClickListener {

        }
        btn_camera_closefilter.setOnClickListener {

        }

        btn_camera_shutter.setOnClickListener {

        }

        btn_camera_switch.setOnClickListener {

        }

        btn_camera_mode.setOnClickListener {

        }

        btn_camera_beauty.setOnClickListener {

        }
        val screenSize =Point()
        windowManager.defaultDisplay.getSize(screenSize)
        val params = glsurfaceview_camera.layoutParams as RelativeLayout.LayoutParams
        params.width= screenSize.x;
        params.height = screenSize.x* 4/3
        glsurfaceview_camera.layoutParams = params
        mCamera = openCamera(glsurfaceview_camera.holder)
        glsurfaceview_camera.holder.addCallback(CameraSurfaceCallback(mCamera))
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        if (grantResults.size !=1 || grantResults[0] ==PackageManager.PERMISSION_GRANTED){
            if (mode == MODE_PIC){
                takePhoto()
            }else{
                takeVideo()
            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }

    fun takePhoto(){


    }

    fun takeVideo(){

    }

    fun openCamera(holder: SurfaceHolder?):Camera?{
        if (mCamera!=null){
            return mCamera
        }
        mCamera = CameraHelper.openCamera(mCameraId)

        mCamera?.let {
            CameraHelper.setOptimalSize(it,mAspectRatio,CameraHelper.getScreenWidth(), CameraHelper.getScreenWidth())
            CameraHelper.setDisplayOritation(this,it,mCameraId)
        }
        return mCamera
    }

}