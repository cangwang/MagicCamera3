package com.werb.library

import com.werb.library.link.RegisterItem
import com.werb.library.link.SoleLinkManager

/**
 * [MoreType] uses in Application to init something
 * Created by wanbo on 2017/7/12.
 */
object MoreType {

    /** [soleRegister] register global RegisterItem [items] RegisterItem Array */
    fun soleRegister(vararg items: RegisterItem) {
        SoleLinkManager.globalRegister(*items)
    }

}