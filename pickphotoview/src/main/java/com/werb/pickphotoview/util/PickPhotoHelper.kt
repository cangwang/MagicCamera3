package com.werb.pickphotoview.util

import android.content.ContentResolver
import android.database.Cursor
import android.provider.MediaStore
import android.util.Log
import com.werb.eventbus.EventBus
import com.werb.pickphotoview.event.PickFinishEvent
import com.werb.pickphotoview.model.DirImage
import com.werb.pickphotoview.model.GroupImage
import com.werb.pickphotoview.model.MediaModel
import com.werb.pickphotoview.model.SelectModel
import java.io.File
import java.util.*
import kotlin.collections.ArrayList


/**
 * Created by wanbo on 2016/12/31.
 */

object PickPhotoHelper {

    val selectImages: ArrayList<SelectModel> by lazy { arrayListOf<SelectModel>() }
    private val mGroupMap = LinkedHashMap<String, ArrayList<MediaModel>>()
    private val dirNames = ArrayList<String>()
    var groupImage: GroupImage? = null
        private set
    var dirImage: DirImage? = null
        private set

    fun start(showGif: Boolean, showVideo: Boolean, resolver: ContentResolver) {
        clear()
        imageThread(showGif, showVideo, resolver).start()
        Log.d("PickPhotoView", "PickPhotoHelper start")
    }

    fun stop() {
        clear()
        Log.d("PickPhotoView", "PickPhotoHelper stop")
    }

    private fun clear() {
        selectImages.clear()
        dirNames.clear()
        mGroupMap.clear()
        groupImage = null
        dirImage = null
    }

    private fun imageThread(showGif: Boolean, showVideo: Boolean, resolver: ContentResolver): Thread {
        return Thread(Runnable {
            val uri = MediaStore.Files.getContentUri("external")

            //jpeg & png & gif & video
            val mCursor: Cursor?
            mCursor = when {
                showGif && showVideo -> {
                    resolver.query(uri, null,
                            """
                            ${MediaStore.Files.FileColumns.MEDIA_TYPE} = ? or
                            ${MediaStore.Files.FileColumns.MEDIA_TYPE} = ?
                        """.trimIndent(),
                            arrayOf("${MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE}", "${MediaStore.Files.FileColumns.MEDIA_TYPE_VIDEO}"),
                            "${MediaStore.Files.FileColumns.DATE_MODIFIED} desc")
                }
                showGif && !showVideo -> {
                    resolver.query(uri, null,
                            """
                            ${MediaStore.Files.FileColumns.MEDIA_TYPE} = ?
                        """.trimIndent(),
                            arrayOf("${MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE}"),
                            "${MediaStore.Files.FileColumns.DATE_MODIFIED} desc")
                }
                !showGif && showVideo -> {
                    resolver.query(uri, null,
                            """
                            (${MediaStore.Files.FileColumns.MEDIA_TYPE} = ? or
                            ${MediaStore.Files.FileColumns.MEDIA_TYPE} = ?) and
                            ${MediaStore.Files.FileColumns.MIME_TYPE} != ?
                        """.trimIndent(),
                            arrayOf("${MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE}", "${MediaStore.Files.FileColumns.MEDIA_TYPE_VIDEO}", "image/gif"),
                            "${MediaStore.Files.FileColumns.DATE_MODIFIED} desc")
                }
                else -> {
                    resolver.query(uri, null,
                            """
                            ${MediaStore.Files.FileColumns.MEDIA_TYPE} = ? and
                            ${MediaStore.Files.FileColumns.MIME_TYPE} != ?
                        """.trimIndent(),
                            arrayOf("${MediaStore.Files.FileColumns.MEDIA_TYPE_IMAGE}", "image/gif"),
                            "${MediaStore.Files.FileColumns.DATE_MODIFIED} desc")
                }
            }

            if (mCursor == null) {
                return@Runnable
            }
            while (mCursor.moveToNext()) {
                // get image path
                val path = mCursor.getString(mCursor.getColumnIndex(MediaStore.Files.FileColumns.DATA))
                val duration = mCursor.getLong(mCursor.getColumnIndex(MediaStore.Video.VideoColumns.DURATION))

                val file = File(path)
                if (!file.exists()) {
                    continue
                }

                // get image parent name
                val parentName = File(path).parentFile.name
//                    Log.d(PickConfig.TAG, parentName + ":" + path)
                // save all Photo
                if (!mGroupMap.containsKey(PickConfig.ALL_PHOTOS)) {
                    dirNames.add(PickConfig.ALL_PHOTOS)
                    val chileList = ArrayList<MediaModel>()
                    chileList.add(MediaModel(path, duration))
                    mGroupMap.put(PickConfig.ALL_PHOTOS, chileList)
                } else {
                    mGroupMap[PickConfig.ALL_PHOTOS]?.add(MediaModel(path, duration))
                }
                // save by parent name
                if (!mGroupMap.containsKey(parentName)) {
                    dirNames.add(parentName)
                    val chileList = ArrayList<MediaModel>()
                    chileList.add(MediaModel(path, duration))
                    mGroupMap.put(parentName, chileList)
                } else {
                    mGroupMap[parentName]?.add(MediaModel(path, duration))
                }
            }
            mCursor.close()
            val groupImage = GroupImage()
            groupImage.mGroupMap = mGroupMap
            val dirImage = DirImage(dirNames)
            this.groupImage = groupImage
            this.dirImage = dirImage
            EventBus.post(PickFinishEvent())
        })
    }
}
