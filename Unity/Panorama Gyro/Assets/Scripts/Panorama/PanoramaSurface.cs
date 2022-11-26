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

    private Texture getPanoramaTexture() {
        // TODO read from Native API the pano image
        return null;
    }
}
