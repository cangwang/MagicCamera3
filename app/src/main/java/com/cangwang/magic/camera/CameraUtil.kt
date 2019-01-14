package com.cangwang.magic.camera

import android.annotation.TargetApi
import android.hardware.Camera
import android.util.Size

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
        for (size in sizes) {
            if (size.height >= height) {
                //找到第一个相等或者小于width的尺寸
                return CameraCompat.CameraSize(size)
            }
        }
        //如果不存在比width还小的，那就返回最小的那个
        return CameraCompat.CameraSize(sizes[sizes.size - 1])
    }
}
