
# Builds library for the interface implementing processing business logic
# from the info comming from Unity.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(
        unity-plugin-interface
        ${CMAKE_CURRENT_SOURCE_DIR}/unityPluginInterface.cpp
)