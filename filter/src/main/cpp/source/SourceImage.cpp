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

#include "SourceImage.h"
#include "Context.hpp"
#include "util.h"

USING_NS_GI

SourceImage* SourceImage::create(int width, int height, const void* pixels) {
    SourceImage* sourceImage = new SourceImage();
    sourceImage->setImage(width, height, pixels);
    return sourceImage;
}

SourceImage* SourceImage::setImage(int width, int height, const void* pixels) {
    this->setFramebuffer(0);
    Framebuffer* framebuffer = Context::getInstance()->getFramebufferCache()->fetchFramebuffer(width, height, true);
    this->setFramebuffer(framebuffer);
    framebuffer->release();

    CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->getFramebuffer()->getTexture()));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
    return this;
}

#if PLATFORM == PLATFORM_IOS
SourceImage* SourceImage::create(NSURL* imageUrl) {
    SourceImage* sourceImage = new SourceImage();
    sourceImage->setImage(imageUrl);
    return sourceImage;
}

SourceImage* SourceImage::setImage(NSURL* imageUrl) {
    NSData *imageData = [[NSData alloc] initWithContentsOfURL:imageUrl];
    setImage(imageData);
    return this;
}

SourceImage* SourceImage::create(NSData* imageData) {
    SourceImage* sourceImage = new SourceImage();
    sourceImage->setImage(imageData);
    return sourceImage;
}

SourceImage* SourceImage::setImage(NSData* imageData) {
    UIImage* inputImage = [[UIImage alloc] initWithData:imageData];
    setImage(inputImage);
    return this;
}

SourceImage* SourceImage::create(UIImage* image) {
    SourceImage* sourceImage = new SourceImage();
    sourceImage->setImage(image);
    return sourceImage;
}

SourceImage* SourceImage::setImage(UIImage* image) {
    UIImage* img = _adjustImageOrientation(image);
    setImage([img CGImage]);
    return this;
}

SourceImage* SourceImage::create(CGImageRef image) {
    SourceImage* sourceImage = new SourceImage();
    sourceImage->setImage(image);
    return sourceImage;
}

SourceImage* SourceImage::setImage(CGImageRef image) {
    GLubyte *imageData = NULL;
    CFDataRef dataFromImageDataProvider = CGDataProviderCopyData(CGImageGetDataProvider(image));
    imageData = (GLubyte *)CFDataGetBytePtr(dataFromImageDataProvider);
    int width = (int)CGImageGetWidth(image);
    int height = (int)CGImageGetHeight(image);
    assert((width > 0 && height > 0) && "image can not be empty");
    
    setImage(width, height, imageData);
    
    CFRelease(dataFromImageDataProvider);
    
    return this;

}

UIImage* SourceImage::_adjustImageOrientation(UIImage* image)
{
    if (image.imageOrientation == UIImageOrientationUp)
        return image;
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    switch (image.imageOrientation) {
        case UIImageOrientationDown:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, image.size.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformRotate(transform, M_PI_2);
            break;
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, 0, image.size.height);
            transform = CGAffineTransformRotate(transform, -M_PI_2);
            break;
        default:
            break;
    }
    
    switch (image.imageOrientation) {
        case UIImageOrientationUpMirrored:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.height, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        default:
            break;
    }
    
    CGContextRef ctx = CGBitmapContextCreate(NULL, image.size.width, image.size.height,
                                             CGImageGetBitsPerComponent(image.CGImage), 0,
                                             CGImageGetColorSpace(image.CGImage),
                                             CGImageGetBitmapInfo(image.CGImage));
    CGContextConcatCTM(ctx, transform);
    switch (image.imageOrientation) {
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            CGContextDrawImage(ctx, CGRectMake(0,0,image.size.height,image.size.width), image.CGImage);
            break;
        default:
            CGContextDrawImage(ctx, CGRectMake(0,0,image.size.width,image.size.height), image.CGImage);
            break;
    }
    
    CGImageRef cgImage = CGBitmapContextCreateImage(ctx);
    UIImage* newImage = [UIImage imageWithCGImage:cgImage];
    CGContextRelease(ctx);
    CGImageRelease(cgImage);
    return newImage;
}

#endif
