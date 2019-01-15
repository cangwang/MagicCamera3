package com.cangwang.magic.camera

import android.annotation.TargetApi
import android.hardware.Camera
import android.util.Size
import com.cangwang.magic.BaseApplication

import java.util.Arrays
import java.util.Collections
import java.util.Comparator

/**
 * Created by 薛贤俊 on 2018/3/7.
 */

object CameraUtil {

    fun findBestSize(height: Int, supportList: List<Camera.Size>): CameraCompat.CameraSize {
        Collections.sort<Camera.Size>(supportList, Comparator<Camera.Size> { o1, o2 ->
            if (o1.width > o2.width) {
                return@Comparator -1
            } else if (o1.width < o2.width) {
                return@Comparator 1
            }
            0
        })

        for (size in supportList) {
            if (size.height <= height) {
                //找到第一个相等或者小于width的尺寸
                return CameraCompat.CameraSize(size)
            }
        }
        //如果不存在比width还小的，那就返回最小的那个
        return CameraCompat.CameraSize(supportList[supportList.size - 1])

    }

    @TargetApi(21)
    fun findBestSize(height: Int, sizes: Array<Size>): CameraCompat.CameraSize {
        Arrays.sort(sizes, Comparator { o1, o2 ->
            if (o1.height < o2.height) {
                return@Comparator -1
            } else if (o1.height > o2.height) {
                return@Comparator 1
            }
            0
        })
        for (i in 1 until sizes.size) {
            if (sizes[i].height >= height) {
                //找到第一个相等或者小于width的尺寸
                return CameraCompat.CameraSize(sizes[i])
            }
        }
        //如果不存在比height还小的，那就返回最小的那个
        return CameraCompat.CameraSize(sizes[sizes.size - 1])
    }

    /**
     * 获取最大的预览大小
     */
    fun getLargePreviewSize(sizes: Array<Size>): CameraCompat.CameraSize{
        var temp = sizes[0]
        val screenWidth = getScreenWidth()
        val screenHeigth = getScreenHeight()
        for (i in 1 until sizes.size) {
            if (sizes[i].width >= screenWidth && sizes[i].height >= screenHeigth)
                temp = sizes[i]
        }
        return CameraCompat.CameraSize(temp)
    }

    fun getScreenWidth():Int{
        return BaseApplication.context.resources.displayMetrics.widthPixels
    }

    fun getScreenHeight():Int{
        return BaseApplication.context.resources.displayMetrics.heightPixels
    }
}
