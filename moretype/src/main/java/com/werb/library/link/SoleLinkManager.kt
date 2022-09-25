package com.werb.library.link

/**
 * [SoleLinkManager] help to register global viewType
 * Created by wanbo on 2017/7/12.
 */
internal object SoleLinkManager {

    var registerItem = arrayOf<RegisterItem>()

    fun globalRegister(vararg items: RegisterItem) {
        @Suppress("UNCHECKED_CAST")
        this.registerItem = items as Array<RegisterItem>
    }
    
}