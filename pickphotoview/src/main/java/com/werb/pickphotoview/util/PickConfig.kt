package com.werb.pickphotoview.util

import com.werb.pickphotoview.R

/**
 * Created by wanbo on 2016/12/30.
 * PickPhotoView 配置文件
 */

object PickConfig {

    // TAG
    const val TAG = "PickPhotoView"
    // intent data
    const val INTENT_PICK_DATA = "intent_pick_Data"
    // intent dirName
    const val INTENT_DIR_NAME = "intent_dir_name"
    // intent img path
    const val INTENT_IMG_PATH = "intent_img_path"
    // intent img list
    const val INTENT_IMG_LIST = "intent_img_list"
    // intent camera uri
    const val INTENT_CAMERA_URI = "intent_camera_uri"
    // intent img select list
    const val INTENT_IMG_LIST_SELECT = "intent_img_list_select"
    // all photos
    const val ALL_PHOTOS = "All Photos"
    // Camera type
    const val CAMERA_TYPE = -1
    // space
    const val ITEM_SPACE = 4
    // intent requestCode
    const val PICK_PHOTO_DATA = 0x5521
    // intent requestCode
    const val CAMERA_PHOTO_DATA = 0x9949
    // intent requestCode
    const val PREVIEW_PHOTO_DATA = 0x7763
    // default size
    const val DEFAULT_PICK_SIZE = 9
    // default span count
    const val DEFAULT_SPAN_COUNT = 4
    // list scroll threshold
    const val SCROLL_THRESHOLD = 30
    // toolbar icon color
    val PICK_BLACK_COLOR = R.color.pick_black
    val PICK_WHITE_COLOR = R.color.pick_white
    // fragment switch
    const val PICK_GIRD = "pick_gird"
    const val PICK_LIST = "pick_list"

    var applyButtonText = "Add"
}
