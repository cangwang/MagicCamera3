package com.cangwang.magic.util

/**
 * openGL辅助类
 * Created by cangwang on 2018/10/12.
 */
object OpenGLJniLib{
    init {
        System.loadLibrary("MagicJni-lib")
    }

    /**
     * 初始化magicBaseView
     */
    external fun magicBaseInit()

    /**
     * 创建magicBaseView
     */
    external fun magicBaseCreate()

    /**
     * 变更触发magicBaseView
     */
    external fun magicBaseChange(width:Int,height:Int)

    /**
     * 绘制
     */
    external fun magicBaseDraw()
}