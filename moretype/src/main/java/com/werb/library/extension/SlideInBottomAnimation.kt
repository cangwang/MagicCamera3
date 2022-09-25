package com.werb.library.extension

import android.animation.Animator
import android.animation.ObjectAnimator
import android.view.View

/**
 * Created by wanbo on 2017/7/13.
 */
class SlideInBottomAnimation : MoreAnimation {

    override fun getItemAnimators(view: View): Array<Animator>{
        return arrayOf(ObjectAnimator.ofFloat(view, "translationY", view.measuredHeight.toFloat(), 0f))
    }
}