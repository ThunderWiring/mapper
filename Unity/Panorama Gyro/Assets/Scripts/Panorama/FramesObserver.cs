using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

/// <summary>
/// Observes the selected frames whether they form a valid panorama by calling the 
/// native plugin.
/// This class also controls the transition between the camera frame and the panorama
/// view scene.
/// </summary>
[RequireComponent(typeof(NativePlugin))]
public class FramesObserver : MonoBehaviour
{
    [SerializeField] private Button panoramButton;

    private NativePlugin plugin;

    private void Awake()
    {
        plugin = GetComponent<NativePlugin>();
    }

    void Start()
    {
        panoramButton.gameObject.SetActive(false);
    }

    void Update()
    {
        // check if native plugin is able to form a valid panorama and display button
        panoramButton.gameObject.SetActive(plugin.isPanoramaImageReady());
    }

    /// <summary>
    /// Callback handler for the panorama button.
    /// </summary>
    public void viewPanoramaScene() {
        
        //should stop camera?

        SceneManager.LoadScene(2);
    }
}
