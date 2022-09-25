package com.werb.library.extension

import android.animation.Animator
import android.animation.ObjectAnimator
import android.view.View

/**
 * Created by wanbo on 2017/7/13.
 */
class AlphaAnimation : MoreAnimation {


    private var startAlpha = 0f

    constructor()

    constructor(startAlpha: Float) {
        this.startAlpha = startAlpha
    }

    override fun getItemAnimators(view: View): Array<Animator> {
        return arrayOf(ObjectAnimator.ofFloat(view, "alpha", startAlpha , 1f))
    }
}