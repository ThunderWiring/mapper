using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

[RequireComponent(typeof(NativePlugin))]
public class PanoramaSurface : MonoBehaviour
{
    // surface panel that displays the panorama.
    [SerializeField] private RawImage panoSurface;
    [SerializeField] private AspectRatioFitter aspectRatioFitter;

    private const float ZOOM = 1.72f;
    private NativePlugin plugin;

    private void Awake()
    {
        Screen.orientation = ScreenOrientation.Portrait;
        plugin = GetComponent<NativePlugin>();

    }
    void Start()
    {
        panoSurface.texture = getPanoramaTexture();
        setFrameAspectRatio();
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.Escape))
        {
            SceneManager.LoadScene(0);
            return;
        }
    }

    private void setFrameAspectRatio()
    {
        // TODO: those numbers should not be hardcoded.
        int width = 1080;
        int height = 1920;
        float ratio = (float)height / (float)width;
        aspectRatioFitter.aspectRatio = ratio;
        panoSurface.rectTransform.localScale = new Vector3(ZOOM, ZOOM, 1f);
        panoSurface.rectTransform.localEulerAngles = new Vector3(0, 0, -90);
    }

    private Texture2D getPanoramaTexture()
    {
        int height = 0, width = 0;
        plugin.getPanoramaDims(ref width, ref height);
        UnityLogger.DLog("getPanoramaTexture - w:{0} h:{1}", width, height);
        Texture2D panoTexture = new Texture2D(width, height, TextureFormat.ARGB32, false);
        panoTexture.Apply();

        plugin.registerTexture(panoTexture);

        UnityLogger.DLog("Created and registered a texture with size {0}x{1}", width, height);
        plugin.sendRenderEvent(RenderEventID.GetPanorama);

        return panoTexture;
    }

}
