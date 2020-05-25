package com.cangwang.magic

import android.annotation.SuppressLint
import android.app.Application
import android.content.Context
import com.google.android.play.core.splitcompat.SplitCompat
import com.google.android.play.core.splitinstall.SplitInstallManager
import com.google.android.play.core.splitinstall.SplitInstallManagerFactory
import com.google.android.play.core.splitinstall.SplitInstallRequest

class BaseApplication: Application() {

    companion object {
        @SuppressLint("StaticFieldLeak")
        lateinit var context:Context

    }

    private var modulePlay:String=""

    private lateinit var manager:SplitInstallManager

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        SplitCompat.install(base)
        manager = SplitInstallManagerFactory.create(base)
        modulePlay = base?.getString(R.string.module_feature_play)?:""
        loadAndLaunchModule(modulePlay)
    }

    fun insatllAllFeaturesNow(){

    }

    fun loadAndLaunchModule(name:String){
        if(manager.installedModules.contains(name)){

        }

        val request = SplitInstallRequest.newBuilder().addModule(name).build()
        manager.startInstall(request)
    }


    override fun onCreate() {
        super.onCreate()
        context = applicationContext
    }
}