package com.cangwang.magic

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ObjectAnimator
import android.animation.ValueAnimator
import android.app.AlertDialog
import android.content.pm.ActivityInfo
import android.content.res.AssetManager
import android.graphics.Point
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.util.Log
import android.view.View
import android.view.Window
import android.view.WindowManager
import android.widget.RelativeLayout
import com.cangwang.magic.adapter.FilterAdapter
import com.cangwang.magic.camera.CameraCompat
import com.cangwang.filter.util.OpenGLJniLib
import com.cangwang.magic.view.CameraFilterSurfaceCallbackV3
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.disposables.Disposable
import kotlinx.android.synthetic.main.activity_camera.*
import kotlinx.android.synthetic.main.filter_layout.*
import java.util.concurrent.TimeUnit

/**
 * 滤镜录制
 * Created by cangwang on 2018/9/12.
 */
class CameraFilterV2Activity : AppCompatActivity() {
    private val TAG = CameraFilterV2Activity::class.java.simpleName
    private val MODE_PIC = 1
    private val MODE_VIDEO = 2
    private var mode = MODE_PIC
    /**
     * 视频最长的时长是15秒
     */
    private val VIDEO_MAX_TIME = 15

    private var mAdapter: FilterAdapter? = null
    private var mSurfaceCallback: CameraFilterSurfaceCallbackV3? = null
    private var beautyLevel: Int = 0

    var mCamera: CameraCompat? = null
    var filterType: Int = 0

    private var videoAnimator: ObjectAnimator? = null
    /**
     * 录像倒计时终止器
     */
    private var mDisposable: Disposable? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        window.setFlags(WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED, WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED)
        window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN)
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        setContentView(R.layout.activity_camera)
        initView()
    }

    private val types = OpenGLJniLib.getFilterTypes()

    private fun initView() {
        filter_listView.layoutManager =
                LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false)
        mAdapter = FilterAdapter(this, types)
        mAdapter?.filterListener = object : FilterAdapter.onFilterChangeListener {
            override fun onFilterChanged(filterType: Int) {
                this@CameraFilterV2Activity.filterType = filterType
                mSurfaceCallback?.setFilterType(filterType)
            }
        }
        filter_listView.adapter = mAdapter
        btn_camera_filter.setOnClickListener {
            showFilters()
        }
        btn_camera_closefilter.setOnClickListener {
            hideFilters()
        }

        btn_camera_shutter.setOnClickListener {
            if (mode == MODE_PIC) {
                takePhoto()
            } else if (mode == MODE_VIDEO) {
                takeVideo()
            }
        }

        btn_camera_switch.setOnClickListener {
            mCamera?.switchCamera()
        }

        btn_camera_mode.setOnClickListener {
            if (mode == MODE_PIC) {
                mode = MODE_VIDEO
                btn_camera_mode.setImageResource(R.drawable.icon_camera)
            } else if (mode == MODE_VIDEO) {
                mode = MODE_PIC
                btn_camera_mode.setImageResource(R.drawable.icon_video)
            }
        }

        btn_camera_beauty.setOnClickListener {
            AlertDialog.Builder(this)
                    .setSingleChoiceItems(arrayOf("关闭", "1", "2", "3", "4", "5"),
                            beautyLevel) { dialog, which ->
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
        Log.d(TAG, "initCamera")
    }

    private fun initCamera() {
        mCamera = CameraCompat.newInstance(this)
        mSurfaceCallback = CameraFilterSurfaceCallbackV3(mCamera, filterType)
        glsurfaceview_camera.holder.addCallback(mSurfaceCallback)
        //初始化摄像头
        mCamera?.startPreview()
    }

    override fun onPause() {
        if (mSurfaceCallback?.isRecording() == true) {
            releaseVideoRecord()
        }
//        mCamera?.stopPreview(true)
        glsurfaceview_camera.holder.removeCallback(mSurfaceCallback)
        mSurfaceCallback?.releaseOpenGL()
        mSurfaceCallback = null
        mCamera = null
        super.onPause()
    }

    private fun takePhoto() {
        mSurfaceCallback?.takePhoto()
    }

    private fun takeVideo() {
        if (mSurfaceCallback?.isRecording() == true) {
            releaseVideoRecord()
        } else {
            startVideoRecord()
        }
    }

    private fun showFilters() {
        val animator = ObjectAnimator.ofInt(layout_filter, "translationY", layout_filter.height, 0)
        animator.duration = 200
        animator.addListener(object : AnimatorListenerAdapter() {
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
        animator.addListener(object : AnimatorListenerAdapter() {
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
        if (layout_filter.visibility == View.VISIBLE) {
            hideFilters()
        } else {
            super.onBackPressed()
        }
    }

    private fun startVideoRecord() {
        showVideoRecord()
        mSurfaceCallback?.startRecordVideo()
        videoAnimator?.start()
    }

    private fun releaseVideoRecord() {
        mSurfaceCallback?.releaseRecordVideo()
        videoAnimator?.end()
        hideVideoRecord()
    }

    private fun showVideoRecord() {
        video_record_seek_bar.visibility = View.VISIBLE
        btn_camera_mode.visibility = View.GONE
        btn_camera_switch.visibility = View.GONE
        btn_camera_beauty.visibility = View.GONE
        btn_camera_filter.visibility = View.GONE
        cutPadding()
        recordCountDown()
    }

    private fun hideVideoRecord() {
        video_record_seek_bar.visibility = View.GONE
        btn_camera_mode.visibility = View.VISIBLE
        btn_camera_switch.visibility = View.VISIBLE
        btn_camera_beauty.visibility = View.VISIBLE
        btn_camera_filter.visibility = View.VISIBLE
        stopRecordCountTime()
    }

    /**
     * 重新设置录像的进度条样式
     */
    private fun cutPadding() {
        val point = Point()
        windowManager.defaultDisplay.getSize(point)
        val width = point.x
        val padding = video_record_seek_bar.paddingLeft
        val layoutParams = video_record_seek_bar.layoutParams as RelativeLayout.LayoutParams
        layoutParams.width = width + padding
        video_record_seek_bar.layoutParams = layoutParams
        video_record_seek_bar.setPadding(0, 0, 0, 0)
    }

    private fun recordCountDown() {
        val count = 15
        mDisposable = Observable.interval(1, 1, TimeUnit.SECONDS)
                .take((count + 1).toLong())
                .map { t ->
                    count - t
                }
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe { aLong ->
                    val time = 16 - aLong!!

                    video_record_seek_bar.progress = time.toInt()
                    if (time == VIDEO_MAX_TIME.toLong()) {
                        releaseVideoRecord()
                    }
                }
    }

    private fun stopRecordCountTime() {
        mDisposable?.dispose()
        mDisposable = null
    }
}