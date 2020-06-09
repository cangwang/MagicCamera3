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

package com.cangwang.magic.util

import android.app.AlertDialog
import android.content.Context
import android.widget.Toast
import com.cangwang.gpuimage.GPUImageFilter

import java.util.LinkedList

object FilterHelper {
    internal val FILTER_BRIGHTNESS = 0
    internal val FILTER_COLOR_INVERT = 1
    internal val FILTER_GRAYSCALE = 2
    internal val FILTER_GAUSSIAN_BLUR = 3
    internal val FILTER_BILATERAL = 4
    internal val FILTER_IOS_BLUR = 5
    internal val FILTER_CANNY_EDGE_DETECTION = 6
    internal val FILTER_WEAK_PIXEL_INCLUSION = 7
    internal val FILTER_NON_MAXIMUM_SUPPRESSION = 8
    internal val FILTER_BEAUTIFY = 9
    internal val FILTER_SOBEL_EDGE_DETECTION = 10
    internal val FILTER_SKETCH = 11
    internal val FILTER_TOON = 12
    internal val FILTER_SMOOTH_TOON = 13
    internal val FILTER_POSTERIZE = 14
    internal val FILTER_PIXELLATION = 15
    internal val FILTER_SATURATION = 16
    internal val FILTER_CONTRAST = 17
    internal val FILTER_EXPOSURE = 18
    internal val FILTER_RGB = 19
    internal val FILTER_HUE = 20
    internal val FILTER_WHITE_BALANCE = 21
    internal val FILTER_LUMINANCE_RANGE = 22
    internal val FILTER_EMBOSS = 23
    internal val FILTER_HALFTONE = 24
    internal val FILTER_CROSSHATCH = 25
    internal val FILTER_SPHERE_REFRACTION = 26
    internal val FILTER_GLASS_SPHERE = 27
    internal val FILTER_SCRATCH_CARD = 28

    private var filterList: FilterList? = null

    private fun getFilterList(): FilterList {
        if (filterList == null) {
            filterList = FilterList()
            filterList!!.addFilter(FILTER_BRIGHTNESS, "Brightness", "BrightnessFilter")
            filterList!!.addFilter(FILTER_COLOR_INVERT, "Color Invert", "ColorInvertFilter")
            filterList!!.addFilter(FILTER_GRAYSCALE, "Grayscale", "GrayscaleFilter")
            filterList!!.addFilter(FILTER_GAUSSIAN_BLUR, "Gaussian Blur", "GaussianBlurFilter")
            filterList!!.addFilter(FILTER_BILATERAL, "Bilateral", "BilateralFilter")
            filterList!!.addFilter(FILTER_IOS_BLUR, "iOS 7 Blur", "IOSBlurFilter")
            filterList!!.addFilter(FILTER_CANNY_EDGE_DETECTION, "Canny Edge Detection",
                    "CannyEdgeDetectionFilter")
            filterList!!.addFilter(FILTER_WEAK_PIXEL_INCLUSION, "Weak Pixel Inclusion",
                    "WeakPixelInclusionFilter")
            filterList!!.addFilter(FILTER_NON_MAXIMUM_SUPPRESSION, "Non Maximum Suppression",
                    "NonMaximumSuppressionFilter")
            filterList!!.addFilter(FILTER_BEAUTIFY, "Beautify", "BeautifyFilter")
            filterList!!.addFilter(FILTER_SOBEL_EDGE_DETECTION, "Sobel Edge Detection",
                    "SobelEdgeDetectionFilter")
            filterList!!.addFilter(FILTER_SKETCH, "Sketch", "SketchFilter")
            filterList!!.addFilter(FILTER_TOON, "Toon", "ToonFilter")
            filterList!!.addFilter(FILTER_SMOOTH_TOON, "Smooth Toon", "SmoothToonFilter")
            filterList!!.addFilter(FILTER_POSTERIZE, "Posterize", "PosterizeFilter")
            filterList!!.addFilter(FILTER_PIXELLATION, "Pixellation", "PixellationFilter")
            filterList!!.addFilter(FILTER_SATURATION, "Saturation", "SaturationFilter")
            filterList!!.addFilter(FILTER_CONTRAST, "Contrast", "ContrastFilter")
            filterList!!.addFilter(FILTER_EXPOSURE, "Exposure", "ExposureFilter")
            filterList!!.addFilter(FILTER_RGB, "RGB Adjustment", "RGBFilter")
            filterList!!.addFilter(FILTER_HUE, "Hue Adjustment", "HueFilter")
            filterList!!.addFilter(FILTER_WHITE_BALANCE, "White Balance", "WhiteBalanceFilter")
            filterList!!.addFilter(FILTER_LUMINANCE_RANGE, "Luminance Range",
                    "LuminanceRangeFilter")
            filterList!!.addFilter(FILTER_EMBOSS, "Emboss Filter", "EmbossFilter")
            filterList!!.addFilter(FILTER_HALFTONE, "Halftone Filter", "HalftoneFilter")
            filterList!!.addFilter(FILTER_CROSSHATCH, "Crosshatch Filter", "CrosshatchFilter")
            filterList!!.addFilter(FILTER_SPHERE_REFRACTION, "Sphere Refraction Filter",
                    "SphereRefractionFilter")
            filterList!!.addFilter(FILTER_GLASS_SPHERE, "Glass Sphere Filter", "GlassSphereFilter")
            filterList!!.addFilter(FILTER_SCRATCH_CARD, "Scatch card Filter", "ScratchCardFilter")
        }
        return filterList!!
    }

    fun showListDialog(
            context: Context,
            listener: OnFilterSelectedListener
    ) {

        val filters = getFilterList()
        val builder = AlertDialog.Builder(context)
        builder.setTitle("Choose a filter")
        builder.setItems(filters.displayNames.toTypedArray()
        ) { dialog, item ->
            Toast.makeText(context, filters.displayNames[item],
                    Toast.LENGTH_SHORT).show()
            listener.OnFilterSelected(createFilter(filters.filters[item]))
        }
        builder.create().show()
    }

    fun applyFilterWithSliderValue(filter: GPUImageFilter, v: Float) {
        var value = v
        val filters = getFilterList()
        val filterClassName = filter.filterClassName
        when (filters.classNames.indexOf(filterClassName)) {
            FILTER_BRIGHTNESS -> {
                run {
                    // let the value between -1 and 1
                    value = (value - 0.5f) * 2.0f
                    filter.setProperty("brightness", value)
                }
            }
            FILTER_COLOR_INVERT -> {
                run { }
            }
            FILTER_GRAYSCALE -> {
                run { }
            }
            FILTER_GAUSSIAN_BLUR -> {
                run {
                    // let the value between 0 - 24
                    value *= 24.0f
                    //filter->setProperty("radius", (int)value);
                    filter.setProperty("sigma", value)
                }
            }
            FILTER_IOS_BLUR -> {
                run {
                    value = value * 10.0f + 1.0f
                    filter.setProperty("downSampling", value)
                }
            }
            FILTER_BILATERAL -> {
                run {
                    // let the value between 0 - 10
                    value *= 10.0f
                    filter.setProperty("distanceNormalizationFactor", value)
                }
            }
            FILTER_CANNY_EDGE_DETECTION -> {
                run {
                    //todo
                }
            }
            FILTER_WEAK_PIXEL_INCLUSION -> {
                run { }
            }
            FILTER_NON_MAXIMUM_SUPPRESSION -> {
                run {
                    value *= 5.0f
                    filter.setProperty("texelSizeMultiplier", value)
                }
            }
            FILTER_BEAUTIFY -> {
                run { }
            }
            FILTER_SOBEL_EDGE_DETECTION -> {
                run { filter.setProperty("edgeStrength", value) }
            }
            FILTER_SKETCH -> {
                run { filter.setProperty("edgeStrength", value) }
            }
            FILTER_TOON -> {
                run {
                    value *= 20.0f
                    filter.setProperty("quantizationLevels", value)
                }
            }
            FILTER_SMOOTH_TOON -> {
                run {
                    // let the value between 1 - 6
                    value = value * 5.0f + 1.0f
                    filter.setProperty("blurRadius", value.toInt())
                }
            }
            FILTER_POSTERIZE -> {
                run {
                    // let the value between 1 and 256
                    value = value * 19.0f + 1.0f
                    filter.setProperty("colorLevels", value.toInt())
                }
            }
            FILTER_PIXELLATION -> {
                run { filter.setProperty("pixelSize", value) }
            }
            FILTER_SATURATION -> {
                run {
                    // let the value between 0 - 2
                    value *= 2.0f
                    filter.setProperty("saturation", value)
                }
            }
            FILTER_CONTRAST -> {
                run {
                    value *= 4.0f
                    filter.setProperty("contrast", value)
                }
            }
            FILTER_EXPOSURE -> {
                run {
                    // let the value between -10 and 10
                    value = (value - 0.5f) * 20.0f
                    filter.setProperty("exposure", value)
                }
            }
            FILTER_RGB -> {
                run {
                    value *= 2.0f
                    filter.setProperty("greenAdjustment", value)
                }
            }
            FILTER_HUE -> {
                run {
                    value *= 360f
                    filter.setProperty("hueAdjustment", value)
                }
            }
            FILTER_WHITE_BALANCE -> {
                run {
                    value = value * 5000.0f + 2500.0f
                    filter.setProperty("temperature", value)
                }
            }
            FILTER_LUMINANCE_RANGE -> {
                run { filter.setProperty("rangeReductionFactor", value) }
            }
            FILTER_EMBOSS -> {
                run {
                    value *= 4.0f
                    filter.setProperty("intensity", value)
                }
            }
            FILTER_HALFTONE -> {
                run {
                    value *= 0.05f
                    filter.setProperty("pixelSize", value)
                }
            }
            FILTER_CROSSHATCH -> {
                run {
                    value = 0.01f + value * 0.05f
                    filter.setProperty("crossHatchSpacing", value)
                }
            }
            FILTER_SPHERE_REFRACTION -> {
                run { filter.setProperty("radius", value) }
            }
            FILTER_GLASS_SPHERE -> {
                run { filter.setProperty("radius", value) }
            }
            FILTER_SCRATCH_CARD -> {
                run {
                    value = value * 5000.0f + 2500.0f
                    filter.setProperty("temperature", value)
                }
            }
            else -> {
            }
        }
    }

    private fun createFilter(type: Int): GPUImageFilter {
        val filters = getFilterList()
        val filterClassName = filters.classNames[type]
        return GPUImageFilter.create(filterClassName)
    }

    private class FilterList {
        var filters: MutableList<Int> = LinkedList()
        var displayNames: MutableList<String> = LinkedList()
        var classNames: MutableList<String> = LinkedList()

        fun addFilter(filter: Int, displayName: String, className: String) {
            filters.add(filter)
            displayNames.add(displayName)
            classNames.add(className)
        }
    }

    interface OnFilterSelectedListener {
        fun OnFilterSelected(newFilter: GPUImageFilter)
    }
}
