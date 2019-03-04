package com.cangwang.magic

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ObjectAnimator
import android.animation.ValueAnimator
import android.app.AlertDialog
import android.content.pm.ActivityInfo
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.view.View
import android.view.Window
import android.view.WindowManager
import com.cangwang.magic.adapter.FilterAdapter
import com.cangwang.magic.camera.CameraCompat
import com.cangwang.magic.util.OpenGLJniLib
import com.cangwang.magic.view.CameraFilterSurfaceCallbackV3
import kotlinx.android.synthetic.main.activity_camera.*
import kotlinx.android.synthetic.main.filter_layout.*

/**
 * Created by cangwang on 2018/9/12.
 */
class CameraFilterV2Activity:AppCompatActivity(){
    private val MODE_PIC = 1
    private val MODE_VIDEO = 2
    private var mode = MODE_PIC

    private var mAdapter: FilterAdapter? = null
    private var mSurfaceCallback:CameraFilterSurfaceCallbackV3?=null
    private var beautyLevel:Int = 0

    var mCamera: CameraCompat?=null

    private var videoAnimator: ObjectAnimator? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        window.setFlags(WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED, WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED)
        window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN)
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        setContentView(R.layout.activity_camera)
        initView()
    }

    private val types = OpenGLJniLib.getFilterTypes()

    private fun initView(){
        filter_listView.layoutManager = LinearLayoutManager(this,LinearLayoutManager.HORIZONTAL,false)
        mAdapter = FilterAdapter(this, types)
        mAdapter?.filterListener= object:FilterAdapter.onFilterChangeListener{
            override fun onFilterChanged(filterType: Int) {
                mSurfaceCallback?.setFilterType(filterType)
            }
        }
        filter_listView.adapter= mAdapter
        btn_camera_filter.setOnClickListener {
            showFilters()
        }
        btn_camera_closefilter.setOnClickListener {
            hideFilters()
        }

        btn_camera_shutter.setOnClickListener {
            if(mode == MODE_PIC) {
                takePhoto()
            }else if (mode == MODE_VIDEO){
                takeVideo()
            }
        }

        btn_camera_switch.setOnClickListener {
            mCamera?.switchCamera()
        }

        btn_camera_mode.setOnClickListener {
            if(mode == MODE_PIC){
                mode = MODE_VIDEO
                btn_camera_mode.setImageResource(R.drawable.icon_camera)
            }else if (mode == MODE_VIDEO){
                mode = MODE_PIC
                btn_camera_mode.setImageResource(R.drawable.icon_video)
            }
        }

        btn_camera_beauty.setOnClickListener {
            AlertDialog.Builder(this)
                    .setSingleChoiceItems(arrayOf("关闭", "1", "2", "3", "4", "5"), beautyLevel) {
                        dialog, which ->
                        beautyLevel = which
                        OpenGLJniLib.setBeautyLevel(which)
                        dialog.dismiss()
                    }
                    .setNegativeButton("取消", null)
                    .show()
        }

        videoAnimator = ObjectAnimator.ofFloat(btn_camera_shutter, "rotation", 0f, 360f)
        videoAnimator?.duration = 500
        videoAnimator?.repeatCount = ValueAnimator.INFINITE
    }

    override fun onResume() {
        super.onResume()
        initCamera()
    }

    private fun initCamera(){
        mCamera = CameraCompat.newInstance(this)
        mSurfaceCallback = CameraFilterSurfaceCallbackV3(mCamera)
        glsurfaceview_camera.holder.addCallback(mSurfaceCallback)
        mCamera?.startPreview()
    }

    override fun onPause() {
        mCamera?.stopPreview(false)
        super.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun takePhoto(){
        mSurfaceCallback?.takePhoto()
    }

    private fun takeVideo(){
        if(mSurfaceCallback?.isRecording() == true) {
            mSurfaceCallback?.releaseRecordVideo()
            videoAnimator?.end()
        }else{
            mSurfaceCallback?.startRecordVideo()
            videoAnimator?.start()
        }
    }

    private fun showFilters() {
        val animator = ObjectAnimator.ofInt(layout_filter, "translationY", layout_filter.height, 0)
        animator.duration = 200
        animator.addListener(object :AnimatorListenerAdapter(){
            override fun onAnimationStart(animation: Animator) {
                btn_camera_shutter.isClickable = false
                layout_filter.visibility = View.VISIBLE
            }
        })

        animator.start()
    }

    private fun hideFilters() {
        val animator = ObjectAnimator.ofInt(layout_filter, "translationY", 0, layout_filter.height)
        animator.duration = 200
        animator.addListener(object :AnimatorListenerAdapter(){
            override fun onAnimationEnd(animation: Animator) {
                // TODO Auto-generated method stub
                layout_filter.visibility = View.INVISIBLE
                btn_camera_shutter.isClickable = true
            }

            override fun onAnimationCancel(animation: Animator) {
                // TODO Auto-generated method stub
                layout_filter.visibility = View.INVISIBLE
                btn_camera_shutter.isClickable = true
            }
        })
        animator.start()
    }

    override fun onBackPressed() {
        if(layout_filter.visibility ==View.VISIBLE){
            hideFilters()
        }else {
            super.onBackPressed()
        }
    }
}