package com.werb.library

import android.view.View
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.werb.library.action.MoreClickListener
import kotlinx.android.extensions.LayoutContainer


/**
 * [MoreViewHolder] Base ViewHolder implement Action fun
 * Created by wanbo on 2017/7/2.
 */
abstract class MoreViewHolder<T : Any>(override val containerView: View) : ViewHolder(containerView), LayoutContainer {

    val injectValueMap = mutableMapOf<String, Any>()

    internal var clickListener: MoreClickListener? = null

    fun getItemView(): View = itemView

    fun addOnTouchListener(viewId: Int) {
        clickListener?.let {
            val _this = it
            itemView.findViewById<View>(viewId).setOnTouchListener { v, event ->
                _this.onItemTouch(v, event, layoutPosition)
            }
        }
    }

    fun addOnTouchListener(view: View) {
        clickListener?.let {
            val _this = it
            view.setOnTouchListener { v, event ->
                _this.onItemTouch(v, event, layoutPosition)
            }
        }
    }

    fun addOnClickListener(viewId: Int) {
        itemView.findViewById<View>(viewId).setOnClickListener { clickListener?.onItemClick(it, layoutPosition) }
    }

    fun addOnClickListener(view: View) {
        view.setOnClickListener { clickListener?.onItemClick(it, layoutPosition) }
    }

    fun addOnLongClickListener(viewId: Int) {
        clickListener?.let { it ->
            val _this = it
            itemView.findViewById<View>(viewId).setOnLongClickListener {
                _this.onItemLongClick(it, layoutPosition)
            }
        }
    }

    fun addOnLongClickListener(view: View) {
        clickListener?.let { it ->
            val _this = it
            view.setOnLongClickListener {
                _this.onItemLongClick(view, layoutPosition)
            }
        }
    }

    /** [bindData] bind data with T  */
    abstract fun bindData(data: T, payloads: List<Any> = arrayListOf())

    /** [unBindData] unbind and release resources*/
    open fun unBindData() {}

}