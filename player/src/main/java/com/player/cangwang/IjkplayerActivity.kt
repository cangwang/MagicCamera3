package com.player.cangwang

import android.graphics.Color
import android.os.Bundle
import android.support.v4.widget.DrawerLayout
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.Toolbar
import android.text.TextUtils
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.TableLayout
import android.widget.TextView
import com.player.cangwang.application.Settings
import com.player.cangwang.content.RecentMediaStorage
import com.player.cangwang.media.AndroidMediaController
import com.player.cangwang.media.IjkVideoView
import com.player.cangwang.media.MeasureHelper
import tv.danmaku.ijk.media.player.IjkMediaPlayer
import tv.danmaku.ijk.media.player.misc.ITrackInfo

class IjkplayerActivity: AppCompatActivity(){

    private var mVideoPath: String? = null
    private var mMediaController: AndroidMediaController? = null
    private var mVideoView: IjkVideoView? = null
    private var mToastTextView: TextView? = null
    private var mHudView: TableLayout? = null
    private var mDrawerLayout: DrawerLayout? = null
    private var mRightDrawer: ViewGroup? = null

    private var mSettings: Settings? = null
    private var mBackPressed: Boolean = false
    private val TAG = IjkplayerActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)
        // handle arguments
        mVideoPath = intent.getStringExtra("videoPath")

        if (!TextUtils.isEmpty(mVideoPath)) {
            RecentMediaStorage(this).saveUrlAsync(mVideoPath)
        }

        // init UI
        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar)

        val actionBar = supportActionBar
        mMediaController = AndroidMediaController(this, false)
        mMediaController?.setSupportActionBar(actionBar)

        mToastTextView = findViewById(R.id.toast_text_view)
        mHudView = findViewById(R.id.hud_view)
        mDrawerLayout = findViewById(R.id.drawer_layout)
        mRightDrawer = findViewById(R.id.right_drawer)

        mDrawerLayout?.setScrimColor(Color.TRANSPARENT)

        // init player
        IjkMediaPlayer.loadLibrariesOnce(null)
        IjkMediaPlayer.native_profileBegin("libijkplayer.so")

        mVideoView = findViewById(R.id.video_view)
        mVideoView?.setMediaController(mMediaController)
        mVideoView?.setHudView(mHudView)
        // prefer mVideoPath
        if (mVideoPath != null)
            mVideoView?.setVideoPath(mVideoPath)
        else {
            Log.e(TAG, "Null Data Source\n")
            finish()
            return
        }
        mVideoView?.start()
    }

    override fun onBackPressed() {
        mBackPressed = true

        super.onBackPressed()
    }

    override fun onStop() {
        super.onStop()

        if (mBackPressed || mVideoView?.isBackgroundPlayEnabled ==false) {
            mVideoView?.stopPlayback()
            mVideoView?.release(true)
            mVideoView?.stopBackgroundPlay()
        } else {
            mVideoView?.enterBackground()
        }
        IjkMediaPlayer.native_profileEnd()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.menu_player, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        val id = item.itemId
        if (id == R.id.action_toggle_ratio) {
            val aspectRatio = mVideoView?.toggleAspectRatio()?:1
            val aspectRatioText = MeasureHelper.getAspectRatioText(this, aspectRatio)
            mToastTextView?.text = aspectRatioText
            mMediaController?.showOnce(mToastTextView as View)
            return true
        } else if (id == R.id.action_toggle_player) {
            val player = mVideoView?.togglePlayer()?:-1
            val playerText = IjkVideoView.getPlayerText(this, player)
            mToastTextView?.text = playerText
            mMediaController?.showOnce(mToastTextView as View)
            return true
        } else if (id == R.id.action_toggle_render) {
            val render = mVideoView?.toggleRender()?:-1
            val renderText = IjkVideoView.getRenderText(this, render)
            mToastTextView?.text = renderText
            mMediaController?.showOnce(mToastTextView as View)
            return true
        } else if (id == R.id.action_show_info) {
            mVideoView?.showMediaInfo()
        } else if (id == R.id.action_show_tracks) {
            if (mDrawerLayout?.isDrawerOpen(mRightDrawer as View) == true) {
                val f = supportFragmentManager.findFragmentById(R.id.right_drawer)
                if (f != null) {
                    val transaction = supportFragmentManager.beginTransaction()
                    transaction.remove(f)
                    transaction.commit()
                }
                mDrawerLayout?.closeDrawer(mRightDrawer as View)
            } else {
//                val f = TracksFragment.newInstance()
////                val transaction = supportFragmentManager.beginTransaction()
////                transaction.replace(R.id.right_drawer, f)
////                transaction.commit()
////                mDrawerLayout?.openDrawer(mRightDrawer as View)
            }
        }

        return super.onOptionsItemSelected(item)
    }

    fun getTrackInfo(): Array<ITrackInfo>? {
        return if (mVideoView == null) null else mVideoView?.trackInfo

    }

    fun selectTrack(stream: Int) {
        mVideoView?.selectTrack(stream)
    }

    fun deselectTrack(stream: Int) {
        mVideoView?.deselectTrack(stream)
    }

    fun getSelectedTrack(trackType: Int): Int {
        return if (mVideoView == null) -1 else mVideoView?.getSelectedTrack(trackType)?:-1

    }
}