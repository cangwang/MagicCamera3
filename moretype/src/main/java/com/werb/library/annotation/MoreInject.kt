package com.werb.library.annotation

/**
 * Created by wanbo on 2018/11/16.
 */
@Retention(AnnotationRetention.RUNTIME)
@Target(AnnotationTarget.FIELD, AnnotationTarget.PROPERTY_GETTER, AnnotationTarget.PROPERTY_SETTER)
annotation class MoreInject(val key: String)