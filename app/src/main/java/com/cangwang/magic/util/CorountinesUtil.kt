package com.cangwang.magic.util

import kotlinx.coroutines.newFixedThreadPoolContext

object CorountinesUtil {
    val Background = newFixedThreadPoolContext(2, "bg")
}