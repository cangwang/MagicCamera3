# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(${CMAKE_SOURCE_DIR} src/main/cpp)
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

add_library( # Sets the name of the library.
             triangle-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/triangle.cpp )

add_library( # Sets the name of the library.
             magicjni

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
        src/main/cpp/magicjni.cpp
        src/main/cpp/utils/OpenglUtils.cpp
        src/main/cpp/utils/Rotation.cpp
        src/main/cpp/utils/TextureRotationUtil.cpp
        src/main/cpp/utils/Matrix.cpp
        src/main/cpp/camera/CameraFilter.cpp
        src/main/cpp/camera/CameraEngine.cpp
        src/main/cpp/image/ImageFilter.cpp
        src/main/cpp/image/ImageInput.cpp
        src/main/cpp/video/VideoFilter.cpp
        src/main/cpp/egl/EGLCore.cpp
        src/main/cpp/filter/advanced/MagicAmaroFilter.cpp
        src/main/cpp/filter/advanced/MagicAntiqueFilter.cpp
        src/main/cpp/filter/advanced/MagicBlackCatFilter.cpp
        src/main/cpp/filter/advanced/MagicBrannanFilter.cpp
        src/main/cpp/filter/advanced/MagicBrooklynFilter.cpp
        src/main/cpp/filter/advanced/MagicBeautyFilter.cpp
        src/main/cpp/filter/advanced/MagicCoolFilter.cpp
        src/main/cpp/filter/advanced/MagicCalmFilter.cpp
        src/main/cpp/filter/advanced/MagicCrayonFilter.cpp
        src/main/cpp/filter/advanced/MagicEarlyBirdFilter.cpp
        src/main/cpp/filter/advanced/MagicEmeraldFilter.cpp
        src/main/cpp/filter/advanced/MagicEvergreenFilter.cpp
        src/main/cpp/filter/advanced/MagicFreudFilter.cpp
        src/main/cpp/filter/advanced/MagicFairytableFilter.cpp
        src/main/cpp/filter/advanced/MagicHefeFilter.cpp
        src/main/cpp/filter/advanced/MagicHealthyFilter.cpp
        src/main/cpp/filter/advanced/MagicHudsonFilter.cpp
        src/main/cpp/filter/advanced/MagicInkwellFilter.cpp
        src/main/cpp/filter/advanced/MagicKevinFilter.cpp
        src/main/cpp/filter/advanced/MagicLomoFilter.cpp
        src/main/cpp/filter/advanced/MagicLookupFilter.cpp
        src/main/cpp/filter/advanced/MagicLatteFilter.cpp
        src/main/cpp/filter/advanced/MagicN1977Filter.cpp
        src/main/cpp/filter/advanced/MagicNashvilleFilter.cpp
        src/main/cpp/filter/advanced/MagicNostalgiaFilter.cpp
        src/main/cpp/filter/advanced/MagicNoneFilter.cpp
        src/main/cpp/filter/advanced/MagicPixarFilter.cpp
        src/main/cpp/filter/advanced/MagicRiseFilter.cpp
        src/main/cpp/filter/advanced/MagicRomanceFilter.cpp
        src/main/cpp/filter/advanced/MagicSakuraFilter.cpp
        src/main/cpp/filter/advanced/MagicSierraFilter.cpp
        src/main/cpp/filter/advanced/MagicSketchFilter.cpp
        src/main/cpp/filter/advanced/MagicSkinWhiteFilter.cpp
        src/main/cpp/filter/advanced/MagicSunriseFilter.cpp
        src/main/cpp/filter/advanced/MagicSunsetFilter.cpp
        src/main/cpp/filter/advanced/MagicSutroFilter.cpp
        src/main/cpp/filter/advanced/MagicSweetsFilter.cpp
        src/main/cpp/filter/advanced/MagicTenderFilter.cpp
        src/main/cpp/filter/advanced/MagicToasterFilter.cpp
        src/main/cpp/filter/advanced/MagicValenciaFilter.cpp
        src/main/cpp/filter/advanced/MagicWaldenFilter.cpp
        src/main/cpp/filter/advanced/MagicWarmFilter.cpp
        src/main/cpp/filter/advanced/MagicWhiteCatFilter.cpp
        src/main/cpp/filter/advanced/MagicXproIIFilter.cpp
        src/main/cpp/filter/gpuimage/GpuImageFilter.cpp
#        src/main/cpp/filter/gpuimage/CameraInputFilter.cpp
        src/main/cpp/filter/gpuimage/CameraInputFilterV2.cpp
        src/main/cpp/filter/MagicFilterFactory.cpp
        src/main/cpp/filter/douyin/MagicSoulOutFilter.cpp
        src/main/cpp/filter/douyin/MagicScaleFilter.cpp
        src/main/cpp/filter/douyin/MagicGlitchFilter.cpp
        src/main/cpp/filter/douyin/MagicShakeEffectFilter.cpp
        src/main/cpp/filter/douyin/MagicShineWhiteFilter.cpp
        src/main/cpp/filter/douyin/MagicVerigoFilter.cpp
        src/main/cpp/filter/douyin/RenderBuffer.cpp)

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

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

target_link_libraries( # Specifies the target library.
                       triangle-lib
                       android
                       GLESv3

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

target_link_libraries( # Specifies the target library.
                       magicjni
                       android
                       GLESv3
                       EGL

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )