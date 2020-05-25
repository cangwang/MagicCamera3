package com.cangwang.magic.util

import android.util.Log
import io.reactivex.Observable
import io.reactivex.disposables.Disposable
import io.reactivex.schedulers.Schedulers
import java.lang.Exception
import java.util.concurrent.LinkedBlockingDeque
import java.util.concurrent.ThreadPoolExecutor
import java.util.concurrent.TimeUnit

object Dipatcher{

    fun singleDipatcher(executor: ThreadPoolExecutor):DipatchImpl{
        return DipatchImpl(executor)
    }

    interface IDipatcher{
        fun dipatch(runnable:()->Unit)
        fun stop(runnable: () -> Unit)
    }

    class IODipatchImpl(minThreadCount:Int,maxThreadCount:Int):IDipatcher{

        private val ioExecutors = Schedulers.from(ThreadPoolExecutor(minThreadCount,maxThreadCount,60L,TimeUnit.SECONDS,LinkedBlockingDeque()))

        override fun dipatch(runnable: () -> Unit) {

        }

        override fun stop(runnable: () -> Unit) {

        }
    }

    class DipatchImpl(executor: ThreadPoolExecutor):IDipatcher{
        private var scheduler = Schedulers.from(executor)
        private var mDispose:Disposable?=null

        override fun dipatch(runnable: () -> Unit) {
            mDispose= Observable.just(1)
                    .observeOn(scheduler)
                    .subscribe{
                        try {
                            runnable.invoke()
                        }catch (e:Exception){
                            Log.e("DipatchImpl",e.toString())
                        }
                    }

        }

        override fun stop(runnable: () -> Unit) {
            if(mDispose?.isDisposed == false){
                mDispose?.dispose()
            }
        }
    }
}