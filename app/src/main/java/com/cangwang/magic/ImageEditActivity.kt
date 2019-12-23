package com.cangwang.magic

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ObjectAnimator
import android.content.pm.ActivityInfo
import android.os.Bundle
import android.view.View
import android.view.Window
import android.view.WindowManager
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import com.cangwang.magic.adapter.FilterAdapter
import com.cangwang.magic.util.OpenGLJniLib
import com.cangwang.magic.view.ImageFilterSurfaceCallback
import com.werb.pickphotoview.model.SelectModel
import com.werb.pickphotoview.util.PickConfig
import kotlinx.android.synthetic.main.activity_album.*
import kotlinx.android.synthetic.main.filter_layout.*

/**
 * 图片编辑
 */
class ImageEditActivity: AppCompatActivity(){

    private var mAdapter: FilterAdapter? = null
    private var mSurfaceCallback: ImageFilterSurfaceCallback?=null
    private var beautyLevel:Int = 0
    private val types = OpenGLJniLib.getFilterTypes()
    private var filterType = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN)
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT
        setContentView(R.layout.activity_album)
        initView()
    }

    override fun onResume() {
        super.onResume()
    }

    private fun initView(){
        filter_listView.layoutManager = LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL,false)
        mAdapter = FilterAdapter(this, types)
        mAdapter?.filterListener= object: FilterAdapter.onFilterChangeListener{
            override fun onFilterChanged(type: Int) {
                filterType = type
                mSurfaceCallback?.setFilterType(type)
            }
        }
        filter_listView.adapter= mAdapter
        initPreview()
        btn_album_filter.setOnClickListener {
            showFilters()
        }
        btn_camera_closefilter.setOnClickListener {
            hideFilters()
        }

        btn_album_save.setOnClickListener {
            mSurfaceCallback?.saveImage()
        }

        btn_album_beauty.setOnClickListener {
            AlertDialog.Builder(this)
                    .setSingleChoiceItems(arrayOf("关闭", "1", "2", "3", "4", "5"), beautyLevel) {
                        dialog, which ->
                        beautyLevel = which
                        OpenGLJniLib.setImageBeautyLevel(which)
                        dialog.dismiss()
                    }
                    .setNegativeButton("取消", null)
                    .show()
        }

    }

    private fun initPreview(){
        val selectPaths = intent.getSerializableExtra(PickConfig.INTENT_IMG_LIST_SELECT) as SelectModel
        mSurfaceCallback = ImageFilterSurfaceCallback(selectPaths.path,filterType)
        album_surfaceview.holder.addCallback(mSurfaceCallback)
    }

    override fun onDestroy() {
        album_surfaceview.holder.removeCallback(mSurfaceCallback)
        mSurfaceCallback?.releaseOpenGL()
        mSurfaceCallback=null
        super.onDestroy()
    }

    private fun showFilters() {
        val animator = ObjectAnimator.ofInt(layout_filter, "translationY", layout_filter.height, 0)
        animator.duration = 200
        animator.addListener(object :AnimatorListenerAdapter(){
            override fun onAnimationStart(animation: Animator) {
                btn_album_save.isClickable = false
                layout_filter.visibility = View.VISIBLE
            }
        })

        animator.start()
    }

    private fun hideFilters() {
        val animator = ObjectAnimator.ofInt(layout_filter, "translationY", 0, layout_filter.height)
        animator.duration = 200
        animator.addListener(object : AnimatorListenerAdapter(){
            override fun onAnimationEnd(animation: Animator) {
                // TODO Auto-generated method stub
                layout_filter.visibility = View.INVISIBLE
                btn_album_save.isClickable = true
            }

            override fun onAnimationCancel(animation: Animator) {
                // TODO Auto-generated method stub
                layout_filter.visibility = View.INVISIBLE
                btn_album_save.isClickable = true
            }
        })
        animator.start()
    }
}