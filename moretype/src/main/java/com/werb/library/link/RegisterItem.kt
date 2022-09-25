package com.werb.library.link

import com.werb.library.MoreViewHolder
import com.werb.library.action.MoreClickListener

/** Created by wanbo <werbhelius@gmail.com> on 2017/9/14. */

data class RegisterItem(val layoutId: Int,
                        val clazzViewHolder: Class<out MoreViewHolder<*>>,
                        var clickListener: MoreClickListener? = null,
                        val injectValue: Map<String, Any>? = null)