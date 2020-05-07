package com.cangwang.magic.camera

data class CameraInfo(
        var previewWidth: Int, var previewHeight: Int, var orientation: Int, var isFront: Boolean,
        var pictureWidth: Int, var pictureHeight: Int
)