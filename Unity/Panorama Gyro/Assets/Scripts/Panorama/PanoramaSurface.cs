using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(NativePlugin))]
public class PanoramaSurface : MonoBehaviour
{
    [SerializeField] private RawImage panoSurface;
    
    private NativePlugin plugin;

    private void Awake()
    {
        plugin = GetComponent<NativePlugin>();

    }
    void Start()
    {
        panoSurface.texture = getPanoramaTexture();
    }

    private Texture2D getPanoramaTexture()
    {
        int height = 0, width = 0;
        plugin.getPanoramaDims(ref width, ref height);
        Texture2D panoTexture = new Texture2D(width, height, TextureFormat.ARGB32, false);
        panoTexture.Apply();

        plugin.registerTexture(panoTexture);

        UnityLogger.DLog("Created and registered a texture with size {0}x{1}", width, height);
        plugin.sendRenderEvent(RenderEventID.GetPanorama);
        return panoTexture;
    }
}
