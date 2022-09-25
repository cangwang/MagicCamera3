package com.werb.library.link

import androidx.recyclerview.widget.DiffUtil

/**
 * Created by wanbo on 2017/12/19.
 */
abstract class XDiffCallback(val oldItem: List<Any>, val newItem: List<Any>) : DiffUtil.Callback()