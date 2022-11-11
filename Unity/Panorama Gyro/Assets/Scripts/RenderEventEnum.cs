using System;
using System.Threading.Tasks;

public enum RenderEventID
{
    // values must correspond with the events defined in the native plugin (UnityRenderEvent).
    RegisterTexture = 1,
    GetPanorama = 2,
}
