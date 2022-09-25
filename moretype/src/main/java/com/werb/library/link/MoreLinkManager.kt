package com.werb.library.link

import android.util.Log
import androidx.collection.SparseArrayCompat
import com.werb.library.MoreViewHolder
import com.werb.library.action.MoreClickListener
import com.werb.library.annotation.MoreInject
import com.werb.library.exception.ModelNotRegisterException
import java.lang.reflect.ParameterizedType


/**
 * Data driven view
 * layout is unique, so we uses layout to link data and view
 * layout id is unique key
 * Created by wanbo on 2017/7/5.
 */
class MoreLinkManager : MoreLink, MoreOperation {

    private val TAG = "MoreType"

    /** [viewHolderMap] save MoreViewHolder by layout id */
    private var viewHolderMap = SparseArrayCompat<Class<out MoreViewHolder<*>>>()

    /** [modelTypeMap] save Data::class by layout id */
    private var modelTypeMap = SparseArrayCompat<Class<*>>()

    /** [modelTypeMap] save clickListener by layout id */
    private var clickListenerMap = SparseArrayCompat<MoreClickListener?>()

    private var injectValueMap = SparseArrayCompat<Map<String, Any>>()

    /** [multiModelMap] save [MultiLink] by Data::class */
    private var multiModelMap = mutableMapOf<Class<*>, MultiLink<Any>>()

    /** [multiViewHolderMap] save MoreViewType by layout id */
    private var multiViewHolderMap = SparseArrayCompat<Class<out MoreViewHolder<*>>>()

    /**
     *  [register] register single MoreViewType , MoreViewType can't repeat
     *  if two ViewType's getViewModel() return same Data::class
     *  [viewHolderMap] and [modelTypeMap] will remove oldViewType and replace by newViewType
     */
    override fun register(registerItem: RegisterItem) {
        val model = reflectClass(registerItem.clazzViewHolder)
        if (modelTypeMap.indexOfValue(model) != -1) {
            val index = modelTypeMap.indexOfValue(model)
            val key = viewHolderMap.keyAt(index)
            val oldViewType = viewHolderMap[key]
            val modelName = model.simpleName
            val newTypeName = registerItem.clazzViewHolder.simpleName
            val oldViewTypeName = oldViewType?.simpleName
            viewHolderMap.removeAt(index)
            modelTypeMap.removeAt(index)
            clickListenerMap.removeAt(index)
            injectValueMap.removeAt(index)
            Log.w(TAG, "model repeated! $modelName.class will replace $oldViewTypeName to $newTypeName")
        }
        @Suppress("UNCHECKED_CAST")
        viewHolderMap.put(registerItem.layoutId, registerItem.clazzViewHolder)
        modelTypeMap.put(registerItem.layoutId, model)
        clickListenerMap.put(registerItem.layoutId, registerItem.clickListener)
        injectValueMap.put(registerItem.layoutId, registerItem.injectValue)
    }

    override fun register(clazz: Class<out MoreViewHolder<*>>, clickListener: MoreClickListener?, injectValue: Map<String, Any>?) {
        val layoutID = requestLayoutID(clazz)
        register(RegisterItem(layoutID, clazz, clickListener, injectValue))
    }

    /**
     * [multiRegister] register multi MoreViewType
     * [clazz] which data will multi register
     * [link] the link between data and kinds of viewType
     * use map save , so same class will be replace, if you register repeated
     */
    @Suppress("UNCHECKED_CAST")
    override fun multiRegister(link: MultiLink<*>) {
        @Suppress("UNCHECKED_CAST")
        multiModelMap.put(reflectClass(link::class.java), link as MultiLink<Any>)
    }

    /**
     * [attachViewTypeLayout] attach to viewType' layout by data item
     * if data belong to single register will return viewType's unique key (layout)
     * if data belong to multi register will return -1
     * if data not register will throw [ModelNotRegisterException]
     */
    override fun attachViewTypeLayout(any: Any): Int {
        val clazz = any.javaClass
        val type = modelTypeMap.indexOfValue(clazz)
        if (type == -1) {
            val multiClazz = multiModelMap.containsKey(clazz)
            if (multiClazz) {
                val multiLink = multiModelMap[clazz]
                multiLink?.let {
                    val link = it.link(any)
                    if (multiViewHolderMap.get(link.layoutId) == null) {
                        multiViewHolderMap.put(link.layoutId, link.clazzViewHolder)
                        clickListenerMap.put(link.layoutId, link.clickListener)
                    }
                    return link.layoutId
                }
            } else {
                throw ModelNotRegisterException(clazz.simpleName as String)
            }
        }
        return viewHolderMap.keyAt(type)
    }

    /** [createViewHolder] return MoreViewHolder in onCreateViewHolder() to create viewHolder*/
    override fun createViewHolder(type: Int): Class<out MoreViewHolder<*>> {
        if (type == -1) {
            throw  NullPointerException("no such type!")
        }

        val viewHolder = viewHolderMap[type]
        val multiViewHolder = multiViewHolderMap[type]

        if (viewHolder != null) {
            return viewHolder
        }

        if (multiViewHolder != null) {
            return multiViewHolder
        }

        throw  NullPointerException("no such type!")
    }

    override fun bindClickListener(holder: MoreViewHolder<*>): MoreClickListener? {
        val clazz = holder.javaClass

        val viewIndex = viewHolderMap.indexOfValue(clazz)
        val multiIndex = multiViewHolderMap.indexOfValue(clazz)

        if (viewIndex != -1) {
            val viewLayout = viewHolderMap.keyAt(viewIndex)
            return clickListenerMap[viewLayout]
        }

        if (multiIndex != -1) {
            val multiLayout = multiViewHolderMap.keyAt(multiIndex)
            return clickListenerMap[multiLayout]
        }

        return null
    }

    /** [userSoleRegister] single register Global ViewType */
    override fun userSoleRegister() {
        val items = SoleLinkManager.registerItem
        for (item in items) {
            register(item)
        }
    }

    fun getinjectValueWithHolder(type: Int): Map<String, Any> {
        val valueMap = injectValueMap[type] ?: return mutableMapOf()
        if (valueMap.isEmpty()) return mutableMapOf()
        return valueMap
    }

    @Suppress("UNCHECKED_CAST")
    private fun reflectClass(clazz: Class<*>): Class<Any> {
        val type = clazz.genericSuperclass as ParameterizedType
        val p = type.actualTypeArguments
        return p[0] as Class<Any>
    }

    private fun requestLayoutID(clazz: Class<*>): Int {
        val layoutID = clazz.getAnnotation(LayoutID::class.java)
        layoutID?.let {
            return layoutID.layoutId
        }
            ?: throw NullPointerException("ViewHolder class ${clazz.simpleName} has not register LayoutID in Annotation !")
    }

}