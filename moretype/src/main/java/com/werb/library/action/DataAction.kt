package com.werb.library.action

import com.werb.library.link.XDiffCallback


/**
 * Action of Data in Adapter
 * Created by wanbo on 2017/7/15.
 */
internal interface DataAction {

    /** [loadData] add data with merge*/
    fun refresh(index: Int = 0, newData : Any, diffUtilClazz: Class<out XDiffCallback>)

    /** [loadData] add data */
    fun loadData(data: Any)

    /** [loadData] add data */
    fun loadData(index: Int ,data: Any)

    /** [getData] get data by position */
    fun getData(position: Int): Any

    /** [getDataIndex] get position by data */
    fun getDataIndex(data: Any) : Int

    /** [removeAllData] remove all data */
    fun removeAllData()

    /** [removeAllNotRefresh] remove all data not notifyDataSetChanged */
    fun removeAllNotRefresh()

    /** [removeDataFromIndex] remove data from index */
    fun removeDataFromIndex(index: Int)

    /** [removeData] remove data by item object */
    fun removeData(data: Any)

    /** [removeData] remove data by position */
    fun removeData(position: Int)

    /** [replaceData] replace data by position */
    fun replaceData(position: Int, data: Any)

}