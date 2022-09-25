package com.werb.library.link

/**
 * Register Layout with Annotation
 * Created by wanbo on 2018/1/8.
 */

@Retention(AnnotationRetention.RUNTIME)
@Target(AnnotationTarget.CLASS, AnnotationTarget.PROPERTY_GETTER, AnnotationTarget.PROPERTY_SETTER)
annotation class LayoutID(val layoutId: Int)