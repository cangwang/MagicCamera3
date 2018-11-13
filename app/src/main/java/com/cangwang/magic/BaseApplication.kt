package com.cangwang.magic

import android.annotation.SuppressLint
import android.app.Application
import android.content.Context

class BaseApplication: Application() {

    companion object {
        @SuppressLint("StaticFieldLeak")
        lateinit var context:Context
    }

    override fun onCreate() {
        super.onCreate()
        context = applicationContext
    }
}