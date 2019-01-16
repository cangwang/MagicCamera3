package com.cangwang.magic

import android.content.pm.ActivityInfo
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.view.Window
import android.view.WindowManager
import com.cangwang.magic.adapter.FilterAdapter
import com.cangwang.magic.util.OpenGLJniLib
import com.cangwang.magic.view.CameraFilterSurfaceCallbackV2
import kotlinx.android.synthetic.main.filter_layout.*

class AlbumActivity:AppCompatActivity(){

    private var mAdapter: FilterAdapter? = null
    private var mSurfaceCallback: CameraFilterSurfaceCallbackV2?=null
    private var beautyLevel:Int = 0
    private val types = OpenGLJniLib.getFilterTypes()

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

    fun initView(){
        filter_listView.layoutManager = LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL,false)
        mAdapter = FilterAdapter(this, types)
        mAdapter?.filterListener= object: FilterAdapter.onFilterChangeListener{
            override fun onFilterChanged(type: Int) {
                mSurfaceCallback?.setFilterType(type)
            }
        }
        filter_listView.adapter= mAdapter
    }
}