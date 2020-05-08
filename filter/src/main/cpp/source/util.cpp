/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "util.h"

#if PLATFORM == PLATFORM_ANDROID
#include <android/log.h>
#elif PLATFORM == PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif


NS_GI_BEGIN

    std::string str_format(const char *fmt,...) {
        std::string strResult="";
        if (NULL != fmt)
        {
            va_list marker;
            va_start(marker, fmt);
            char *buf = 0;
            int result = vasprintf (&buf, fmt, marker);
            if (!buf)
            {
                va_end(marker);
                return strResult;
            }

            if (result < 0)
            {
                free (buf);
                va_end(marker);
                return strResult;
            }

            result = (int)strlen (buf);
            strResult.append(buf,result);
            free(buf);
            va_end(marker);
        }
        return strResult;
    }

    void Log(const std::string& tag, const std::string& format, ...)
    {
        char buffer[10240];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format.c_str(), args);
        va_end(args);
#if PLATFORM == PLATFORM_ANDROID
        __android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s", buffer);
#elif PLATFORM == PLATFORM_IOS
        NSLog(@"%s", buffer);
#endif
        
    }

NS_GI_END
