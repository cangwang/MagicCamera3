package com.werb.library.extension

import android.animation.Animator
import android.view.View

/**
 *  support custom Animation
 * Created by wanbo on 2017/7/13.
 */
interface MoreAnimation {

    /** [getItemAnimators] Array of Animator uses to display recyclerView Item */
    fun getItemAnimators(view: View): Array<Animator>

}