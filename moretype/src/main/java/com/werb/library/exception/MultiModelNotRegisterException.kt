package com.werb.library.exception

/**
 * throw this Exception when not register MultiViewType for data
 * Created by wanbo on 2017/7/12.
 */
class MultiModelNotRegisterException(className: String)
    : RuntimeException("$className.class has another viewType and you not register , checkout multiRegister() with $className.class")