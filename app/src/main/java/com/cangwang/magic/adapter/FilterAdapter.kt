package com.cangwang.magic.adapter

import android.content.Context
import android.support.v7.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.TextView

import com.cangwang.magic.R
import com.cangwang.magic.helper.FilterTypeHelper
import com.cangwang.magic.helper.MagicFilterType

/**
 * Created by why8222 on 2016/3/17.
 */
class FilterAdapter(private val context: Context, private val filters: IntArray) : RecyclerView.Adapter<FilterAdapter.FilterHolder>() {
    private var selected = 0

    interface onFilterChangeListener {
        fun onFilterChanged(filterType: Int)
    }

    var filterListener: onFilterChangeListener? = null

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FilterHolder {
        val view = LayoutInflater.from(context).inflate(R.layout.filter_item_layout,
                parent, false)
        return FilterHolder(view)
    }

    override fun onBindViewHolder(holder: FilterHolder, position: Int) {
        holder.thumbImage.setImageResource(FilterTypeHelper.FilterType2Thumb(filters!![position]))
        holder.filterName.setText(FilterTypeHelper.FilterType2Name(filters[position]))
        holder.filterName.setBackgroundColor(context.resources.getColor(
                FilterTypeHelper.FilterType2Color(filters[position])))
        if (position == selected) {
            holder.thumbSelected.visibility = View.VISIBLE
            holder.thumbSelected_bg.setBackgroundColor(context.resources.getColor(
                    FilterTypeHelper.FilterType2Color(filters[position])))
            holder.thumbSelected_bg.alpha = 0.7f
        } else {
            holder.thumbSelected.visibility = View.GONE
        }

        holder.filterRoot.setOnClickListener { v ->
            if (selected != position) {

                val lastSelected = selected
                selected = position
                notifyItemChanged(lastSelected)
                notifyItemChanged(position)
                filterListener?.onFilterChanged(filters[position])
            }
        }
    }

    override fun getItemCount(): Int {
        return filters?.size ?: 0
    }

    inner class FilterHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var thumbImage: ImageView = itemView.findViewById(R.id.filter_thumb_image)
        var filterName: TextView = itemView.findViewById(R.id.filter_thumb_name)
        var thumbSelected: FrameLayout = itemView.findViewById(R.id.filter_root)
        var filterRoot: FrameLayout = itemView.findViewById(R.id.filter_thumb_selected)
        var thumbSelected_bg: View = itemView.findViewById(R.id.filter_thumb_selected_bg)
    }
}
