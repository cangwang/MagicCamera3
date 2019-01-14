package com.cangwang.magic

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.support.v4.app.ActivityCompat
import android.support.v4.content.PermissionChecker
import android.view.View
import android.widget.Button
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
        const val CAMERA_REQ = 1
        const val CAMERA_FILTER = 2
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        findViewById<Button>(R.id.button_camera).setOnClickListener { v ->
            if (PermissionChecker.checkSelfPermission(this@MainActivity, Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.CAMERA), CAMERA_REQ)
            } else {
                startActivity(CAMERA_REQ)
            }
        }
        findViewById<Button>(R.id.button_filter).setOnClickListener {
            if (PermissionChecker.checkSelfPermission(this@MainActivity, Manifest.permission.CAMERA) == PackageManager.PERMISSION_DENIED ||
                    PermissionChecker.checkSelfPermission(this@MainActivity, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED) {
                ActivityCompat.requestPermissions(this@MainActivity, arrayOf(Manifest.permission.CAMERA,Manifest.permission.WRITE_EXTERNAL_STORAGE), CAMERA_FILTER)
            } else {
                startActivity(CAMERA_FILTER)
            }
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>,
                                            grantResults: IntArray) {
        if (requestCode == CAMERA_REQ && grantResults.size != 1 || grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            startActivity(CAMERA_REQ)
        }else if (requestCode == CAMERA_FILTER && grantResults[0] == PackageManager.PERMISSION_GRANTED && grantResults[1] == PackageManager.PERMISSION_GRANTED){
            startActivity(CAMERA_FILTER)
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    private fun startActivity(id: Int) {
        when (id) {
            CAMERA_REQ -> startActivity(Intent(this, CameraActivity::class.java))
            CAMERA_FILTER -> startActivity(Intent(this,CameraFilterActivity::class.java))
            else -> {
            }
        }
    }
}
