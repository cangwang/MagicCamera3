package com.cangwang.filter.util

import android.content.res.AssetManager
import android.util.Log
import android.view.Surface
import java.lang.Exception

/**
 * openGL辅助类
 * Created by cangwang on 2018/10/12.
 */
object OpenGLJniLib {
    init {
        try {
            System.loadLibrary("magicjni")
        } catch (e: Exception) {
            Log.e("OpenGLJniLib", e.toString())
        }
    }

    /**
     * 初始化magicBaseView
     */
    external fun magicBaseInit(
            surface: Surface, width: Int, height: Int, manager: AssetManager
    ): Int

    /**
     * 创建magicBaseView
     */
    external fun magicBaseCreate()

    /**
     * 变更触发magicBaseView
     */
    external fun magicBaseChange(width: Int, height: Int)

    /**
     * 绘制
     */
    external fun magicBaseDraw(matrix: FloatArray)

    external fun magicBaseRelease()

    /**
     * 调整大小
     */
    external fun magicAdjustSize(orientation: Int, isFront: Boolean, flipVertical: Boolean)

    /**
     * 相机滤镜创建
     */
    external fun magicFilterCreate(surface: Surface, manager: AssetManager): Int

    external fun magicFilterChange(width: Int, height: Int)

    external fun magicFilterDraw(matrix: FloatArray, isTakePhoto: String)

    external fun magicFilterRelease()

    /**
     * 图片滤镜创建
     */
    external fun magicImageFilterCreate(
            surface: Surface, manager: AssetManager, path: String, degree: Int
    ): Int

    external fun magicImageFilterChange(width: Int, height: Int)

    external fun magicImageFilterDraw(matrix: FloatArray, isSavePhoto: String)

    external fun magicImageFilterRelease()

    /**
     * 设置美颜级别
     */
    external fun setImageBeautyLevel(level: Int)

    /**
     * 获取滤镜列表
     */
    external fun getFilterType(): IntArray

    /**
     * 设置滤镜类型
     */
    external fun setFilterType(type: Int)

    external fun setImageFilterType(type: Int)

    /**
     * 设置美颜级别
     */
    external fun setBeautyLevel(level: Int)

    /**
     * 摄像头保存图片
     */
    external fun savePhoto(address: String): Boolean

    /**
     * 编辑保存图片
     */
    external fun saveImage(address: String): Boolean

    /**
     * 创建视频Surface
     */
    external fun buildVideoSurface(surface: Surface, textureId: Int, manager: AssetManager): Int

    external fun magicVideoFilterChange(width: Int, height: Int)

    /**
     * 视频绘制
     */
    external fun magicVideoDraw(matrix: FloatArray, time: Long)

    /**
     * 设置滤镜类型
     */
    external fun setVideoFilterType(type: Int)

    /**
     * 释放视频Surface
     */
    external fun releaseVideoSurface()
}