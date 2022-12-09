using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

[RequireComponent(typeof(NativePlugin))]
public class GyroPanorama : TouchableObject
{
    [SerializeField] private GameObject sphere;
    [SerializeField] private Material material;

    private Renderer sphereRenderer;
    private NativePlugin rendererPlugin;
    private Texture2D writableTexture;

    void Awake()
    {
        rendererPlugin = GetComponent<NativePlugin>();
    }

    // Start is called before the first frame update
    void Start()
    {
        if (sphere == null)
        {
            UnityLogger.DLog("sphere is null");
        }
        sphereRenderer = sphere.GetComponent<Renderer>();
        if (sphereRenderer == null)
        {
            UnityLogger.DLog("Sphere has no renderer");
            return;
        }
        Texture2D tex = getPanoramTexture();
        sphereRenderer.material.SetTexture("_MainTex", tex);
    }

    protected override void onEscape()
    {
        SceneManager.LoadScene(0);
    }

    private Texture2D getPanoramTexture()
    {
        int height = 0, width = 0;
        rendererPlugin.getPanoramaDims(ref width, ref height);
        Texture2D panoTexture = new Texture2D(width, height, TextureFormat.ARGB32, false);
        panoTexture.Apply();

        rendererPlugin.registerTexture(panoTexture);
        UnityLogger.DLog("Created and registered a texture with size {0}x{1}", width, height);
        rendererPlugin.sendRenderEvent(RenderEventID.GetPanorama);
        return panoTexture;
    }


}
