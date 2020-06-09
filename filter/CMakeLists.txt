# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${CMAKE_SOURCE_DIR} src/main/cpp)
include_directories(glm)

set(SRC_DIR src/main/cpp)
#file(GLOB_RECURSE CPP_SRCS "${SRC_DIR}/*.cpp")  #指定当前目录下的所有.cpp文件（包括子目录）
file(GLOB_RECURSE CPP_SRCS
        "${SRC_DIR}/bitmap/*.cpp"
        "${SRC_DIR}/camera/*.cpp"
        "${SRC_DIR}/egl/*.cpp"
        "${SRC_DIR}/filter/*.cpp"
        "${SRC_DIR}/image/*.cpp"
        "${SRC_DIR}/utils/*.cpp"
        "${SRC_DIR}/video/*.cpp"
        "${SRC_DIR}/magicjni.cpp")

add_library( # Sets the name of the library.
        magicjni

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${CPP_SRCS})



#file(GLOB_RECURSE Files_CPP
#        "${SRC_DIR}/effect/*.cpp"
#        "${SRC_DIR}/source/*.cpp"
#        "${SRC_DIR}/GPUImagexJNI.cpp")
#
#add_library( # Sets the name of the library.
#        GPUImage-x
#
#        # Sets the library as a shared library.
#        SHARED
#
#        # Provides a relative path to your source file(s).
#        ${Files_CPP})

#一定要保证加载的顺序，因为_registerFilterClass有先后顺序，不然System.loadLibrary会崩溃
add_library( GPUImage-x
        SHARED
        src/main/cpp/source/Ref.cpp
        src/main/cpp/source/util.cpp
        src/main/cpp/source/FramebufferCache.cpp
        src/main/cpp/source/Framebuffer.cpp
        src/main/cpp/source/GLProgram.cpp
        src/main/cpp/source/Context.cpp
        src/main/cpp/source/math.cpp
        src/main/cpp/GPUImagexJNI.cpp
        src/main/cpp/source/Source.cpp
        src/main/cpp/source/SourceImage.cpp
        src/main/cpp/source/SourceCamera.cpp
        src/main/cpp/source/Target.cpp
        src/main/cpp/source/TargetView.cpp
        src/main/cpp/effect/Filter.cpp
        src/main/cpp/effect/FilterGroup.cpp
        src/main/cpp/effect/BrightnessFilter.cpp
        src/main/cpp/effect/ColorInvertFilter.cpp
        src/main/cpp/effect/GrayscaleFilter.cpp
        src/main/cpp/effect/GaussianBlurFilter.cpp
        src/main/cpp/effect/GaussianBlurMonoFilter.cpp
        src/main/cpp/effect/NearbySampling3x3Filter.cpp
        src/main/cpp/effect/DirectionalSobelEdgeDetectionFilter.cpp
        src/main/cpp/effect/DirectionalNonMaximumSuppressionFilter.cpp
        src/main/cpp/effect/WeakPixelInclusionFilter.cpp
        src/main/cpp/effect/CannyEdgeDetectionFilter.cpp
        src/main/cpp/effect/BilateralFilter.cpp
        src/main/cpp/effect/ColorMatrixFilter.cpp
        src/main/cpp/effect/HSBFilter.cpp
        src/main/cpp/effect/BeautifyFilter.cpp
        src/main/cpp/effect/SobelEdgeDetectionFilter.cpp
        src/main/cpp/effect/SketchFilter.cpp
        src/main/cpp/effect/ToonFilter.cpp
        src/main/cpp/effect/PixellationFilter.cpp
        src/main/cpp/effect/SaturationFilter.cpp
        src/main/cpp/effect/ContrastFilter.cpp
        src/main/cpp/effect/ExposureFilter.cpp
        src/main/cpp/effect/RGBFilter.cpp
        src/main/cpp/effect/HueFilter.cpp
        src/main/cpp/effect/WhiteBalanceFilter.cpp
        src/main/cpp/effect/SmoothToonFilter.cpp
        src/main/cpp/effect/PosterizeFilter.cpp
        src/main/cpp/effect/LuminanceRangeFilter.cpp
        src/main/cpp/effect/IOSBlurFilter.cpp
        src/main/cpp/effect/NonMaximumSuppressionFilter.cpp
        src/main/cpp/effect/SingleComponentGaussianBlurMonoFilter.cpp
        src/main/cpp/effect/SingleComponentGaussianBlurFilter.cpp
        src/main/cpp/effect/Convolution3x3Filter.cpp
        src/main/cpp/effect/EmbossFilter.cpp
        src/main/cpp/effect/HalftoneFilter.cpp
        src/main/cpp/effect/CrosshatchFilter.cpp
        src/main/cpp/effect/SphereRefractionFilter.cpp
        src/main/cpp/effect/GlassSphereFilter.cpp
        src/main/cpp/effect/ScratchCardFilter.cpp
        )

#add_library( # Sets the name of the library.
#             native-lib
#
#             # Sets the library as a shared library.
#             SHARED
#
#             # Provides a relative path to your source file(s).
#             rc/main/cpp/native-lib.cpp)
#
#add_library( # Sets the name of the library.
#             triangle-lib
#
#             # Sets the library as a shared library.
#             SHARED
#
#             # Provides a relative path to your source file(s).
#             src/main/cpp/triangle.cpp)



# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#target_link_libraries( # Specifies the target library.
#                       native-lib
#
#                       # Links the target library to the log library
#                       # included in the NDK.
#                       ${log-lib} )
#
#target_link_libraries( # Specifies the target library.
#                       triangle-lib
#                       android
#                       GLESv3
#
#                       # Links the target library to the log library
#                       # included in the NDK.
#                       ${log-lib} )

target_link_libraries( # Specifies the target library.
        magicjni
        android
        GLESv3
        EGL
        jnigraphics

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )

target_link_libraries(
        GPUImage-x
        ${log-lib}
        GLESv3
        jnigraphics )
