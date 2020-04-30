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

#ifndef GPUIMAGE_X_SOURCECAMERA_H
#define GPUIMAGE_X_SOURCECAMERA_H


#include "Source.hpp"

#if PLATFORM == PLATFORM_IOS
#import <AVFoundation/AVFoundation.h>

@class VideoDataOutputSampleBufferDelegate;
#endif

NS_GI_BEGIN

class SourceCamera : public Source{
public:
    SourceCamera();
    virtual ~SourceCamera();
    
    static SourceCamera* create();

    void setFrameData(int width, int height, const void* pixels, RotationMode outputRotation = RotationMode::NoRotation);
#if PLATFORM == PLATFORM_IOS    
    bool init();
    bool init(NSString* sessionPreset, AVCaptureDevicePosition cameraPosition);
    static bool isCameraExist(AVCaptureDevicePosition cameraPosition);
    void start();
    void stop();
    void pause();
    void resume();
    bool isRunning();
    bool flip();
    
    AVCaptureDevicePosition getCameraPosition();
    void setOutputImageOrientation(UIInterfaceOrientation orientation);
    void setHorizontallyMirrorFrontFacingCamera(bool newValue);
    void setHorizontallyMirrorRearFacingCamera(bool newValue);
#endif

private:
#if PLATFORM == PLATFORM_IOS
    VideoDataOutputSampleBufferDelegate* _videoDataOutputSampleBufferDelegate;
    AVCaptureSession* _captureSession;
    BOOL _capturePaused;
    GPUImage::RotationMode _outputRotation;
    //GPUImage::RotationMode internalRotation;
    AVCaptureDeviceInput* _captureDeviceInput;
    AVCaptureVideoDataOutput* _captureVideoDataOutput;
    /// This determines the rotation applied to the output image, based on the source material
    UIInterfaceOrientation _outputImageOrientation;
    /// These properties determine whether or not the two camera orientations should be mirrored. By default, both are NO.
    bool _horizontallyMirrorFrontFacingCamera, _horizontallyMirrorRearFacingCamera;
    void _updateOutputRotation();
#endif
};

NS_GI_END

#endif //GPUIMAGE_X_SOURCECAMERA_H

#if PLATFORM == PLATFORM_IOS
@interface VideoDataOutputSampleBufferDelegate : NSObject  <AVCaptureVideoDataOutputSampleBufferDelegate>
@property (nonatomic) GPUImage::SourceCamera* sourceCamera;
@property (nonatomic) GPUImage::RotationMode rotation;
@end
#endif
