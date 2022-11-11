using System.Collections;
using System;
using UnityEngine;
using System.Runtime.InteropServices;

public class NativePlugin : MonoBehaviour
{
    struct RotationInfo
    {
        public float w, xs, ys, zs;
        public float eulerX, eulerY, eulerZ;
    }


#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void SetTextureFromUnity(IntPtr texture, int w, int h);

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern IntPtr GetRenderEventFunc();

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void receiveImage(Color32[] pixels, int width, int height);

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void paintTexturePanorama();


#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void getPanoDims(ref int width, ref int height);

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void receiveGyroImage(
        Color32[] pixels, int width, int height, float w, float xs, float ys, float zs);

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("NativeRenderingPlugin")]
#endif
    private static extern void receiveGyroImage(
        Color32[] pixels, int width, int height, RotationInfo rotInfo);

    public void registerTexture(Texture texture)
    {
        if (texture == null)
        {
            Debug.Log("[registerTexture] Passing null texture to native plugin");
            return;
        }
        SetTextureFromUnity(texture.GetNativeTexturePtr(), texture.width, texture.height);
    }

    public void sendCameraFramesToPlugin(
        Color32[] pixels, int width, int height, Quaternion q)
    {
        var rot = getRotationInfo(q);
        UnityLogger.DLog("receiveGyroImage ");
        receiveGyroImage(pixels, width, height, rot);
    }

    private RotationInfo getRotationInfo(Quaternion q) {
        var info = new RotationInfo();
        info.w = q.w;
        info.xs = q.x;
        info.ys = q.y;
        info.zs = q.z;
        info.eulerX = q.eulerAngles.x;
        info.eulerY = q.eulerAngles.y;
        info.eulerZ = q.eulerAngles.z;
        return info;
    }
    public void getPanoramaDims(ref int width, ref int height)
    {
        getPanoDims(ref width, ref height);
    }

    public void sendRenderEvent(RenderEventID eventId)
    {
        GL.IssuePluginEvent(GetRenderEventFunc(), (int)eventId);
        //GC.Collect();
    }

}
