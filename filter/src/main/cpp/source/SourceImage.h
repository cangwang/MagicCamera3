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

#ifndef GPUIMAGE_X_SOURCEIMAGE_H
#define GPUIMAGE_X_SOURCEIMAGE_H

#include "Source.hpp"

NS_GI_BEGIN

class SourceImage : public Source{
public:
    SourceImage() {}

    static SourceImage* create(int width, int height, const void* pixels);
    SourceImage* setImage(int width, int height, const void* pixels);

#if PLATFORM == PLATFORM_IOS
    static SourceImage* create(NSURL* imageUrl);
    SourceImage* setImage(NSURL* imageUrl);
    
    static SourceImage* create(NSData* imageData);
    SourceImage* setImage(NSData* imageData);
    
    static SourceImage* create(UIImage* image);
    SourceImage* setImage(UIImage* image);
    
    static SourceImage* create(CGImageRef image);
    SourceImage* setImage(CGImageRef image);

private:
    UIImage* _adjustImageOrientation(UIImage* image);
#endif
};

NS_GI_END

#endif //GPUIMAGE_X_SOURCEIMAGE_H
