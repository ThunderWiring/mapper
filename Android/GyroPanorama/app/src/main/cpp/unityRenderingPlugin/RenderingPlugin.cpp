
#include "PlatformBase.h"
#include "RenderAPI.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <thread>
#include "../logger.h"
#include "unityPluginInterface.h"
#include "renderEventEnum.h"

#define EXPORTED extern "C"

static void *g_TextureHandle = nullptr;
static int g_TextureWidth = 0;
static int g_TextureHeight = 0;
static RenderAPI *s_CurrentAPI = nullptr;
static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;
static IUnityInterfaces *s_UnityInterfaces = nullptr;
static IUnityGraphics *s_Graphics = nullptr;
static UnityPluginInterface pluginInterface;

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

EXPORTED void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces *unityInterfaces) {
    s_UnityInterfaces = unityInterfaces;
    s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
    s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

#if SUPPORT_VULKAN
    if (s_Graphics->GetRenderer() == kUnityGfxRendererNull)
    {
        extern void RenderAPI_Vulkan_OnPluginLoad(IUnityInterfaces*);
        RenderAPI_Vulkan_OnPluginLoad(unityInterfaces);
    }
#endif // SUPPORT_VULKAN
    // Run OnGraphicsDeviceEvent(initialize) manually on plugin load
    OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

EXPORTED void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload() {
    s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

#if UNITY_WEBGL
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();

extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterPlugin()
{
    UnityRegisterRenderingPlugin(UnityPluginLoad, UnityPluginUnload);
}
#endif

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType) {
    LOGD("RenderingPlugin", "OnGraphicsDeviceEvent: eventType = %d", eventType);
    // Create graphics API implementation upon initialization
    if (eventType == kUnityGfxDeviceEventInitialize) {
        assert(s_CurrentAPI == nullptr);
        s_DeviceType = s_Graphics->GetRenderer();
        s_CurrentAPI = CreateRenderAPI(s_DeviceType);
        LOGD("RenderingPlugin", "OnGraphicsDeviceEvent: s_DeviceType = %d", s_DeviceType);
    }

    // Let the implementation process the device related events
    if (s_CurrentAPI) {
        s_CurrentAPI->ProcessDeviceEvent(eventType, s_UnityInterfaces);
    }

    // Cleanup graphics API implementation upon shutdown
    if (eventType == kUnityGfxDeviceEventShutdown) {
        delete s_CurrentAPI;
        s_CurrentAPI = nullptr;
        s_DeviceType = kUnityGfxRendererNull;
    }
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventID) {
    LOGD("RenderingPlugin", "eventID = %d", eventID);
    if (s_CurrentAPI == nullptr) {
        LOGD("RenderingPlugin", "OnRenderEvent: s_CurrentAPI is null");
        return;
    }
    void *textureHandle = g_TextureHandle;
    int width = g_TextureWidth;
    int height = g_TextureHeight;
    switch (eventID) {
        case 2:
            pluginInterface.getPanorama(s_CurrentAPI, textureHandle, width, height);
            break;
        case 1:
        default:
            break;
    }
}

/**
 * Resets all the images taken so far as well as the calculated panorama.
 * Calling this function will be in the same state as when user opens the app for first time.
 * */
EXPORTED void resetImagesBuffer() {
    pluginInterface.resetImages();
}

/**
 * Returns the panorama dimensions back to Unity code.
 */
EXPORTED void getPanoDims(int &width, int &height) {
    LOGD("RenderingPlugin", "getting pano dims");
    pluginInterface.getPanoramaDims(width, height);
}

/** Gets the image from Unity into the plugin and the camera rotation when the image was taken
 * @param pixels pointer to the image pixel data
 * @param width, height dimensions of the image
 * @param (w, xs, ys, zs) the quaternion vector for the rotation.
 * */
EXPORTED void receiveGyroImage(
        UnityColor32 *pixels, int width, int height, RotationInfo rotInfo) {
    if (!pixels) {
        LOGE("RenderingPlugin", "receiveGyroImage - null pixels array");
        return;
    }
    pluginInterface.getImageFromUnity(
            Quaternion(rotInfo), pixels, width, height);
}

/** Gets the image from Unity into the plugin
 * @param pixels pointer to the image pixel data
 * @param width, height dimensions of the image
 * */
EXPORTED void receiveImage(UnityColor32 *pixels, int width, int height) {
    if (!pixels) {
        LOGE("RenderingPlugin", "receiveImage - null pixels array");
        return;
    }
    LOGD("RenderingPlugin", "receiveImage, pixel size: %d, dims (%d, %d)",
         sizeof(UnityColor32), width, height);
    pluginInterface.getImageFromUnity(pixels, width, height);
}

EXPORTED void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
SetTextureFromUnity(void *textureHandle, int w, int h) {
    LOGD("RenderingPlugin", "SetTextureFromUnity - width: %d, height: %d, ptr: %x", w, h,
         textureHandle);
    g_TextureHandle = textureHandle;
    g_TextureWidth = w;
    g_TextureHeight = h;
}

EXPORTED void paintTexturePanorama() {
    void *textureHandle = g_TextureHandle;
    int width = g_TextureWidth;
    int height = g_TextureHeight;
    LOGD("RenderingPlugin", "paintTexturePanorama width = %d, height = %d, ptr: %x", width, height,
         textureHandle);
    pluginInterface.getPanorama(s_CurrentAPI, textureHandle, width, height);
}

EXPORTED UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc() {
    return OnRenderEvent;
}
