package com.cangwang.magic.adapter

import android.annotation.SuppressLint
import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

import com.cangwang.magic.R
import com.cangwang.magic.helper.FilterTypeHelper
import com.cangwang.magic.helper.MagicFilterType

/**
 * Created by why8222 on 2016/3/17.
 */
class ImageFilterAdapter(private val context: Context, private val filters: IntArray) : RecyclerView.Adapter<ImageFilterAdapter.FilterHolder>() {
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

    override fun onBindViewHolder(holder: FilterHolder, @SuppressLint("RecyclerView") position: Int) {
        holder.thumbImage.setImageResource(FilterTypeHelper.FilterType2Thumb(filters[position]))
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
        return filters.size
    }

    inner class FilterHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        var thumbImage: ImageView
        var filterName: TextView
        var thumbSelected: FrameLayout
        var filterRoot: FrameLayout
        var thumbSelected_bg: View

        init {
            thumbImage = itemView.findViewById(R.id.filter_thumb_image)
            filterName = itemView.findViewById(R.id.filter_thumb_name)
            thumbSelected = itemView.findViewById(R.id.filter_thumb_selected)
            filterRoot = itemView.findViewById(R.id.filter_root)
            thumbSelected_bg = itemView.findViewById(R.id.filter_thumb_selected_bg)
        }
    }
}
