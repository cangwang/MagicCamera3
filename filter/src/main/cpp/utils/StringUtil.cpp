#include "StringUtil.h"

#define LOG_TAG "StringUtil"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)


template <typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}
