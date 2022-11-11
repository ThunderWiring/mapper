
add_subdirectory(pluginInterface)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Unity)

add_library(
        unity-rendering-plugin
        RenderAPI.cpp
        RenderAPI_D3D12.cpp
        RenderAPI_D3D11.cpp
        RenderAPI_OpenGLCoreES.cpp
        RenderAPI_Vulkan.cpp
        RenderingPlugin.cpp
)
