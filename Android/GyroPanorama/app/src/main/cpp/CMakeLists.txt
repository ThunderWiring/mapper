# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.10.2)
project("gyropanorama")

set(OpenCV_STATIC on)
set(OpenCV_DIR $ENV{OPENCV_ANDROID}/sdk/native/jni)
find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/image_warp)
include_directories(${PROJECT_SOURCE_DIR}/geometry)
include_directories(${PROJECT_SOURCE_DIR}/unityRenderingPlugin)
include_directories(${PROJECT_SOURCE_DIR}/unityRenderingPlugin/pluginInterface)
include_directories(${PROJECT_SOURCE_DIR}/unityRenderingPlugin/Unity)


add_library(
        NativeRenderingPlugin
        SHARED
        unityRenderingPlugin/RenderAPI.cpp
        unityRenderingPlugin/RenderAPI_D3D11.cpp
        unityRenderingPlugin/RenderAPI_D3D12.cpp
        unityRenderingPlugin/RenderAPI_OpenGLCoreES.cpp
        unityRenderingPlugin/RenderAPI_Vulkan.cpp
        unityRenderingPlugin/RenderingPlugin.cpp
        unityRenderingPlugin/pluginInterface/unityPluginInterface.cpp
)

add_library(
        imageWarp
        SHARED
        ${PROJECT_SOURCE_DIR}/image_warp/pano_builder.cpp
        ${PROJECT_SOURCE_DIR}/image_warp/image_reader.cpp
        ${PROJECT_SOURCE_DIR}/image_warp/quaternion.cpp
)

find_library(log-lib log)
find_library(native-media-lib mediandk)
find_library(android-lib android)
find_library(jnigraphics-lib jnigraphics)
find_library(gl-lib GLESv2)


target_link_libraries(
        imageWarp
        ${gl-lib}
        ${jnigraphics-lib}
        ${OpenCV_LIBS}
)

target_link_libraries(
        NativeRenderingPlugin
        imageWarp
)