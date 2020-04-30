package com.cangwang.magic

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.support.v4.app.ActivityCompat
import android.support.v4.content.PermissionChecker
import android.widget.Toast
import com.cangwang.magic.util.CorountinesUtil
import com.werb.pickphotoview.PickPhotoView
import com.werb.pickphotoview.model.SelectModel
import com.werb.pickphotoview.util.PickConfig
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.CoroutineStart
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.newFixedThreadPoolContext
import kotlinx.coroutines.runBlocking
import java.util.ArrayList
import kotlin.coroutines.CoroutineContext

class MainActivity : AppCompatActivity(), CoroutineScope by CoroutineScope(Dispatchers.Default) {
    companion object {
        const val CAMERA_REQ = 1
        const val CAMERA_FILTER = 2
        const val ALBUM_REQ = 3
        const val CAMERA_EFFECT = 4
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        button_camera.setOnClickListener { v ->
            if (PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity,
                        arrayOf(Manifest.permission.CAMERA), CAMERA_REQ)
            } else {
                startActivity(CAMERA_REQ)
            }
        }
        button_filter.setOnClickListener {
            if (PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED ||
                    PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED ||
                    PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity,
                        arrayOf(Manifest.permission.CAMERA,
                                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                Manifest.permission.RECORD_AUDIO), CAMERA_FILTER)
            } else {
                startActivity(CAMERA_FILTER)
            }
        }
        button_album.setOnClickListener {
            if (PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity,
                        arrayOf(Manifest.permission.CAMERA), ALBUM_REQ)
            } else {
                startActivity(ALBUM_REQ)
            }
        }
        button_effect.setOnClickListener {
            if (PermissionChecker.checkSelfPermission(this@MainActivity,
                            Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity,
                        arrayOf(Manifest.permission.CAMERA), CAMERA_EFFECT)
            } else {
                startActivity(CAMERA_EFFECT)
            }
        }
    }

    override fun onRequestPermissionsResult(
            requestCode: Int, permissions: Array<String>,
            grantResults: IntArray
    ) {
        if (requestCode == CAMERA_REQ && grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            startActivity(CAMERA_REQ)
        } else if (requestCode == CAMERA_FILTER && grantResults[0] == PackageManager.PERMISSION_GRANTED
                && grantResults[1] == PackageManager.PERMISSION_GRANTED
                && grantResults[2] == PackageManager.PERMISSION_GRANTED) {
            startActivity(CAMERA_FILTER)
        } else if (requestCode == ALBUM_REQ && grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            startActivity(ALBUM_REQ)
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    private fun startActivity(id: Int) {
        when (id) {
            CAMERA_REQ -> startActivity(Intent(this, CameraActivity::class.java))
            CAMERA_FILTER -> startActivity(Intent(this, CameraFilterV2Activity::class.java))
            ALBUM_REQ -> {
                PickPhotoView.Builder(this@MainActivity)
                        .setPickPhotoSize(1)
                        .setClickSelectable(
                                true)             // click one image immediately close and return image
                        .setShowCamera(true)
                        .setHasPhotoSize(7)
                        .setAllPhotoSize(10)
                        .setSpanCount(3)
                        .setLightStatusBar(false)
                        .setShowGif(false)                    // is show gif
                        .setShowVideo(false)
                        .start()
            }
            CAMERA_EFFECT -> startActivity(Intent(this, CameraSampleActivity::class.java))
            else -> {
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == 0) {
            return
        }
        if (data == null) {
            return
        }
        if (requestCode == PickConfig.PICK_PHOTO_DATA) {
            val selectPaths = data.getSerializableExtra(
                    PickConfig.INTENT_IMG_LIST_SELECT) as ArrayList<SelectModel>
            if (selectPaths.size > 0) {
                val intent = Intent(this, ImageEditActivity::class.java)
                intent.putExtra(PickConfig.INTENT_IMG_LIST_SELECT, selectPaths[0])
                startActivity(intent)
            } else {
                Toast.makeText(this, "choose no picture", Toast.LENGTH_SHORT).show()
            }
        }
    }
}
