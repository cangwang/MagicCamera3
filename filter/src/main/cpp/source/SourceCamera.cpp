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

#include "SourceCamera.h"
#include "Context.hpp"
#include "util.h"

USING_NS_GI

SourceCamera::SourceCamera() {
#if PLATFORM == PLATFORM_IOS
    _videoDataOutputSampleBufferDelegate = [[VideoDataOutputSampleBufferDelegate alloc] init];
    _videoDataOutputSampleBufferDelegate.sourceCamera = this;
    
    _horizontallyMirrorFrontFacingCamera = false;
    _horizontallyMirrorRearFacingCamera = false;
#endif
}

SourceCamera::~SourceCamera() {
#if PLATFORM == PLATFORM_IOS
    stop();
    _videoDataOutputSampleBufferDelegate = 0;
#endif
}

SourceCamera* SourceCamera::create() {
    SourceCamera* sourceCamera = new SourceCamera();
#if PLATFORM == PLATFORM_IOS
    if (!sourceCamera->init()) {
        sourceCamera = 0;
    }
#endif
    return sourceCamera;
}

void SourceCamera::setFrameData(int width, int height, const void* pixels, RotationMode outputRotation/* = RotationMode::NoRotation*/) {
    this->setFramebuffer(0);
    Framebuffer* framebuffer = Context::getInstance()->getFramebufferCache()->fetchFramebuffer(width, height, true);
    this->setFramebuffer(framebuffer, outputRotation);
    framebuffer->release();

    CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->getFramebuffer()->getTexture()));
#if PLATFORM == PLATFORM_IOS
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels));
#elif PLATFORM == PLATFORM_ANDROID
	CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
#endif
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

#if PLATFORM == PLATFORM_IOS
bool SourceCamera::init() {
    if (isCameraExist(AVCaptureDevicePositionFront))
        return init(AVCaptureSessionPreset640x480, AVCaptureDevicePositionFront);
    else
        return init(AVCaptureSessionPreset640x480, AVCaptureDevicePositionBack);
}

bool SourceCamera::init(NSString* sessionPreset, AVCaptureDevicePosition cameraPosition) {
    _outputRotation = GPUImage::NoRotation;
    //internalRotation = GPUImage::NoRotation;
    _capturePaused = NO;
    
    _captureSession = [[AVCaptureSession alloc] init];
    _captureSession.sessionPreset = sessionPreset;
    
    // input
    AVCaptureDevice* device = 0;
    for(AVCaptureDevice* dev in [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo])
    {
        if([dev position] == cameraPosition)
        {
            device = dev;
            break;
        }
    }
    if (!device) return false;
    
    NSError *error = nil;
    _captureDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if ([_captureSession canAddInput:_captureDeviceInput])
    {
        [_captureSession addInput:_captureDeviceInput];
    } else {
        return false;
    }
    
    // output
    _captureVideoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    [_captureVideoDataOutput setAlwaysDiscardsLateVideoFrames:YES];
    [_captureSession addOutput:_captureVideoDataOutput];
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    [_captureVideoDataOutput setSampleBufferDelegate:_videoDataOutputSampleBufferDelegate queue:queue];
    _captureVideoDataOutput.videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], kCVPixelBufferPixelFormatTypeKey,
                                 nil];
    
    setOutputImageOrientation(UIInterfaceOrientationPortrait);
    
    return true;
}

bool SourceCamera::isCameraExist(AVCaptureDevicePosition cameraPosition) {
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices)
    {
        if ([device position] == cameraPosition)
            return true;
    }
    return false;
}

void SourceCamera::start() {
    if (![_captureSession isRunning])
    {
        _videoDataOutputSampleBufferDelegate.sourceCamera = this;
        [_captureSession startRunning];
    };
}

void SourceCamera::stop() {
    if ([_captureSession isRunning])
    {
        _videoDataOutputSampleBufferDelegate.sourceCamera = 0;
        [_captureSession stopRunning];
    }
}

void SourceCamera::pause() {
    _capturePaused = true;
}

void SourceCamera::resume() {
    _capturePaused = false;
}

bool SourceCamera::isRunning() {
    return [_captureSession isRunning];
}

bool SourceCamera::flip() {
    AVCaptureDevicePosition cameraPosition = [[_captureDeviceInput device] position];
    if (cameraPosition == AVCaptureDevicePositionBack)
    {
        cameraPosition = AVCaptureDevicePositionFront;
    }
    else
    {
        cameraPosition = AVCaptureDevicePositionBack;
    }

    if (!isCameraExist(cameraPosition))
        return false;
    
    AVCaptureDevice* device = 0;
    for(AVCaptureDevice* dev in [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo])
    {
        if([dev position] == cameraPosition)
        {
            device = dev;
            break;
        }
    }
    if (!device) return false;
    
    NSError *error = nil;
    AVCaptureDeviceInput* newCaptureDeviceInput = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if (!newCaptureDeviceInput) return false;
    
    [_captureSession beginConfiguration];
    
    [_captureSession removeInput:_captureDeviceInput];
    if ([_captureSession canAddInput:newCaptureDeviceInput])
    {
        [_captureSession addInput:newCaptureDeviceInput];
        _captureDeviceInput = newCaptureDeviceInput;
    }
    else
    {
        [_captureSession addInput:_captureDeviceInput];
    }
    [_captureSession commitConfiguration];
    
    _updateOutputRotation();
    
    return true;
}

AVCaptureDevicePosition SourceCamera::getCameraPosition()
{
    return [[_captureDeviceInput device] position];
}

void SourceCamera::setOutputImageOrientation(UIInterfaceOrientation orientation) {
    _outputImageOrientation = orientation;
    _updateOutputRotation();
}

void SourceCamera::setHorizontallyMirrorFrontFacingCamera(bool newValue)
{
    _horizontallyMirrorFrontFacingCamera = newValue;
    _updateOutputRotation();
}

void SourceCamera::setHorizontallyMirrorRearFacingCamera(bool newValue)
{
    _horizontallyMirrorRearFacingCamera = newValue;
    _updateOutputRotation();
}

void SourceCamera::_updateOutputRotation()
{
    if (getCameraPosition() == AVCaptureDevicePositionBack)
    {
        if (_horizontallyMirrorRearFacingCamera)
        {
            switch(_outputImageOrientation)
            {
                case UIInterfaceOrientationPortrait:
                    _outputRotation = GPUImage::RotateRightFlipVertical; break;
                case UIInterfaceOrientationPortraitUpsideDown:
                    _outputRotation = GPUImage::Rotate180; break;
                case UIInterfaceOrientationLandscapeLeft:
                    _outputRotation = GPUImage::FlipHorizontal; break;
                case UIInterfaceOrientationLandscapeRight:
                    _outputRotation = GPUImage::FlipVertical; break;
                default:
                    _outputRotation = GPUImage::NoRotation;
            }
        }
        else
        {
            switch(_outputImageOrientation)
            {
                case UIInterfaceOrientationPortrait:
                    _outputRotation = GPUImage::RotateRight; break;
                case UIInterfaceOrientationPortraitUpsideDown:
                    _outputRotation = GPUImage::RotateLeft; break;
                case UIInterfaceOrientationLandscapeLeft:
                    _outputRotation = GPUImage::Rotate180; break;
                case UIInterfaceOrientationLandscapeRight:
                    _outputRotation = GPUImage::NoRotation; break;
                default:
                    _outputRotation = GPUImage::NoRotation;
            }
        }
    }
    else
    {
        if (_horizontallyMirrorFrontFacingCamera)
        {
            switch(_outputImageOrientation)
            {
                case UIInterfaceOrientationPortrait:
                    _outputRotation = GPUImage::RotateRightFlipVertical; break;
                case UIInterfaceOrientationPortraitUpsideDown:
                    _outputRotation = GPUImage::RotateRightFlipHorizontal; break;
                case UIInterfaceOrientationLandscapeLeft:
                    _outputRotation = GPUImage::FlipHorizontal; break;
                case UIInterfaceOrientationLandscapeRight:
                    _outputRotation = GPUImage::FlipVertical; break;
                default:
                    _outputRotation = GPUImage::NoRotation;
            }
        }
        else
        {
            switch(_outputImageOrientation)
            {
                case UIInterfaceOrientationPortrait:
                    _outputRotation = GPUImage::RotateRight; break;
                case UIInterfaceOrientationPortraitUpsideDown:
                    _outputRotation = GPUImage::RotateLeft; break;
                case UIInterfaceOrientationLandscapeLeft:
                    _outputRotation = GPUImage::NoRotation; break;
                case UIInterfaceOrientationLandscapeRight:
                    _outputRotation = GPUImage::Rotate180; break;
                default:
                    _outputRotation = GPUImage::NoRotation;
            }
        }
    }
    _videoDataOutputSampleBufferDelegate.rotation = _outputRotation;
}
#endif


#if PLATFORM == PLATFORM_IOS
@implementation VideoDataOutputSampleBufferDelegate
#pragma mark AVCaptureVideoDataOutputSampleBufferDelegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    if (_sourceCamera) {
        Context::getInstance()->runSync([&]{
            CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
            CVPixelBufferLockBaseAddress(imageBuffer, 0);
            _sourceCamera->setFrameData((int) CVPixelBufferGetWidth(imageBuffer),
                                        (int) CVPixelBufferGetHeight(imageBuffer),
                                        CVPixelBufferGetBaseAddress(imageBuffer),
                                        _rotation);
            CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
            _sourceCamera->proceed();
        });
    }
}
@end
#endif
