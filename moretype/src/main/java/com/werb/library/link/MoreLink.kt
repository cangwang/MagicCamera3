package com.werb.library.link

import com.werb.library.MoreViewHolder
import com.werb.library.action.MoreClickListener


/**
 * [MoreLink] link model and view
 * Created by wanbo on 2017/7/2.
 */
interface MoreLink {

    /** [register]  register single link viewType when we use with click listener */
    fun register(registerItem: RegisterItem)

    /** [register]  register single link viewType when we use with click listener with Annotation*/
    fun register(clazz: Class<out MoreViewHolder<*>>, clickListener: MoreClickListener? = null, injectValue: Map<String, Any>? = null)

    /** [multiRegister]  register multi link like one2more viewType when we use */
    fun multiRegister(link: MultiLink<*>)

    /** [userSoleRegister] register sole global viewType */
    fun userSoleRegister()
}

internal interface MoreOperation {
    /** [attachViewTypeLayout]  find viewType layout by item of list */
    fun attachViewTypeLayout(any: Any): Int

    /** [createViewHolder]  createViewHolder to show in list */
    fun createViewHolder(type: Int): Class<out MoreViewHolder<*>>

    /** [bindClickListener]  bindClickListener with holder */
    fun bindClickListener(holder: MoreViewHolder<*>): MoreClickListener?

}