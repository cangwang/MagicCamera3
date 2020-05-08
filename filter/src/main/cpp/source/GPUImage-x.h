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

#ifndef GPUImage_x_h
#define GPUImage_x_h


#include "Context.hpp"
#include "Framebuffer.hpp"
#include "FramebufferCache.hpp"
#include "GLProgram.hpp"
#include "macros.h"
#include "math.hpp"
#include "Ref.hpp"
#include "util.h"
#include "source/Source.hpp"
#include "source/SourceImage.h"
#include "source/SourceCamera.h"
#include "target/Target.hpp"
#include "target/TargetView.h"
#if PLATFORM == PLATFORM_IOS
#include "target/iOS/IOSTarget.hpp"
#include "target/iOS/GPUImageTarget.h"
#include "target/iOS/GPUImageView.h"
#endif

#include "filter/Filter.hpp"
#include "filter/FilterGroup.hpp"
#include "filter/BrightnessFilter.hpp"
#include "filter/BilateralFilter.hpp"
#include "filter/BeautifyFilter.hpp"
#include "filter/CannyEdgeDetectionFilter.hpp"
#include "filter/ColorInvertFilter.hpp"
#include "filter/ColorMatrixFilter.hpp"
#include "filter/NonMaximumSuppressionFilter.hpp"
#include "filter/DirectionalNonMaximumSuppressionFilter.hpp"
#include "filter/DirectionalSobelEdgeDetectionFilter.hpp"
#include "filter/GaussianBlurFilter.hpp"
#include "filter/GaussianBlurMonoFilter.hpp"
#include "filter/SingleComponentGaussianBlurFilter.hpp"
#include "filter/SingleComponentGaussianBlurMonoFilter.hpp"
#include "filter/GrayscaleFilter.hpp"
#include "filter/HSBFilter.hpp"
#include "filter/NearbySampling3x3Filter.hpp"
#include "filter/WeakPixelInclusionFilter.hpp"
#include "filter/SobelEdgeDetectionFilter.hpp"
#include "filter/SketchFilter.hpp"
#include "filter/ToonFilter.hpp"
#include "filter/SmoothToonFilter.hpp"
#include "filter/PosterizeFilter.hpp"
#include "filter/PixellationFilter.hpp"
#include "filter/SaturationFilter.hpp"
#include "filter/ContrastFilter.hpp"
#include "filter/ExposureFilter.hpp"
#include "filter/RGBFilter.hpp"
#include "filter/HueFilter.hpp"
#include "filter/WhiteBalanceFilter.hpp"
#include "filter/LuminanceRangeFilter.hpp"
#include "filter/IOSBlurFilter.hpp"
#include "filter/Convolution3x3Filter.hpp"
#include "filter/EmbossFilter.hpp"
#include "filter/HalftoneFilter.hpp"
#include "filter/CrosshatchFilter.hpp"
#include "filter/SphereRefractionFilter.hpp"
#include "filter/GlassSphereFilter.hpp"

#endif /* GPUImage_x_h */
