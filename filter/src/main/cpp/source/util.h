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

#ifndef util_h
#define util_h

#include <stdlib.h>
#include <string>
#include <string.h>
#include "macros.h"
#include <stdarg.h>

NS_GI_BEGIN

    std::string str_format(const char *fmt,...);
    void Log(const std::string& tag, const std::string& format, ...);

#define rotationSwapsSize(rotation) ((rotation) == GPUImage::RotateLeft || (rotation) == GPUImage::RotateRight || (rotation) == GPUImage::RotateRightFlipVertical || (rotation) == GPUImage::RotateRightFlipHorizontal)

NS_GI_END

#endif /* util_h */
