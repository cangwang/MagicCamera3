package com.cangwang.magic

import android.content.pm.PackageManager
import android.graphics.Point
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.widget.RelativeLayout
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
        windowManager.defaultDisplay.getSize(screenSize);
        val params = glsurfaceview_camera.layoutParams as RelativeLayout.LayoutParams
        params.width= screenSize.x;
        params.height = screenSize.x* 4/3
        glsurfaceview_camera.layoutParams = params
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
}