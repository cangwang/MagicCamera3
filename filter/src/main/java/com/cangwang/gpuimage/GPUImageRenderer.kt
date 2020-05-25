package com.cangwang.gpuimage

import android.opengl.GLSurfaceView
import java.util.LinkedList
import java.util.Queue

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GPUImageRenderer : GLSurfaceView.Renderer {
    private var mPreDrawQueue = LinkedList<Runnable>()
    private var mDrawQueue = LinkedList<Runnable>()
    private var mPostDrawQueue = LinkedList<Runnable>()
    private var mSource: GPUImageSource? = null

    fun setSource(source: GPUImageSource?) {
        mSource = source
    }

    override fun onDrawFrame(gl: GL10?) {
        runAll(mPreDrawQueue)
        runAll(mDrawQueue)
        runAll(mPostDrawQueue)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        mSource?.proceed(true, false)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
    }

    fun runAll(queue: Queue<Runnable>) {
        synchronized(queue) {
            while (!queue.isEmpty()) {
                queue.poll().run()
            }
        }
    }

    fun isPreDrawQueueEmpty(): Boolean {
        synchronized(mPreDrawQueue) {
            return mPreDrawQueue.isEmpty()
        }
    }

    fun runOnPreDraw(runnable: Runnable) {
        synchronized(mPreDrawQueue) {
            mPreDrawQueue.add(runnable)
        }
    }

    fun isDrawQueueEmpty(): Boolean {
        synchronized(mDrawQueue) {
            return mDrawQueue.isEmpty()
        }
    }

    fun runOnDraw(runnable: Runnable) {
        synchronized(mDrawQueue) {
            mDrawQueue.add(runnable)
        }
    }

    fun isPostDrawQueueEmpty(): Boolean {
        synchronized(mPostDrawQueue) {
            return mPostDrawQueue.isEmpty()
        }
    }

    fun runOnPostDraw(runnable: Runnable) {
        synchronized(mPostDrawQueue) {
            mPostDrawQueue.add(runnable)
        }
    }

    fun clear() {
        mPreDrawQueue.clear()
        mDrawQueue.clear()
        mPostDrawQueue.clear()
    }
}