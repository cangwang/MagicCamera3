package com.werb.library.extension

import android.animation.Animator
import android.animation.ObjectAnimator
import android.view.View

/**
 * Created by wanbo on 2017/7/13.
 */
class ScaleInAnimation : MoreAnimation {

    constructor()
    constructor(startScale: Float) {
        this.startScale = startScale
    }

    private var startScale = .5f

    override fun getItemAnimators(view: View): Array<Animator> {
        return arrayOf(ObjectAnimator.ofFloat(view, "scaleX", startScale, 1f),
                ObjectAnimator.ofFloat(view, "scaleY", startScale, 1f))
    }
}