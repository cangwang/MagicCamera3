package com.werb.library.extension

import com.werb.library.MoreAdapter
import com.werb.library.MoreViewHolder

/**
 * Created by wanbo on 2017/7/13.
 */
internal interface AnimExtension {

    fun renderWithAnimation(): MoreAdapter

    fun renderWithAnimation(animation: MoreAnimation): MoreAdapter

    fun addAnimation(holder: MoreViewHolder<Any>)

    fun duration(duration: Long): MoreAdapter

    fun firstShowAnim(firstShow: Boolean) : MoreAdapter

    fun startAnimPosition(position: Int): MoreAdapter

}