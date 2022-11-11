using UnityEngine;
using UnityEngine.Android;
using UnityEngine.SceneManagement;
using UnityEngine.UI;


enum CameraDeviceStatus
{
    NotInitialized, Recording, Paused
}

/// <summary>
/// Controls the device camera and reads frames from the main camera for processing.
/// </summary>
[RequireComponent(typeof(GyroController), typeof(NativePlugin))]
public class CameraDevice : MonoBehaviour
{
    [SerializeField] private Button cameraButton;
    [SerializeField] private RawImage background;
    [SerializeField] private AspectRatioFitter aspectRatioFitter;

    private const float ZOOM = 0.43f;
    private const int MIN_WIDTH_THRESH = 100; // used to indicate whether the camera frame is being received whole.

    private bool shouldStartRecording = false;
    private CameraDeviceStatus cameraState = CameraDeviceStatus.NotInitialized;
    private WebCamTexture backCam;
    private GyroController gyroController;
    private NativePlugin plugin;

    private void Awake()
    {
        Screen.orientation = ScreenOrientation.Portrait;
        gyroController = GetComponent<GyroController>();
        plugin = GetComponent<NativePlugin>();
    }

    void Update()
    {
        if (shouldStartRecording)
        {
            updateCameraFrame();
        }
    }

    private void onRecordingStop()
    {
        backCam.Stop();
        SceneManager.LoadScene(1); // pano virtual 360 view
    }

    private void updateCameraFrame()
    {
        if (cameraState == CameraDeviceStatus.NotInitialized || backCam == null)
        {
            startCameraDevice();
        }
        else if (cameraState == CameraDeviceStatus.Paused)
        {
            backCam.Play();
            background.texture = backCam;
        }
        else if (backCam.width < MIN_WIDTH_THRESH)
        {
            Debug.Log(string.Format("Still is waiting another frame for correct info. Frame dims: ({0}, {1}", backCam.width, backCam.height));
            return;
        }
        setFrameAspectRatio();


        var frame = backCam.GetPixels32();
        plugin.sendCameraFramesToPlugin(
            frame, backCam.width, backCam.height, gyroController.rot);
    }

    private void setFrameAspectRatio()
    {
        float ratio = (float)backCam.width / (float)backCam.height;
        aspectRatioFitter.aspectRatio = ratio;
        float scaleY = backCam.videoVerticallyMirrored ? -ZOOM : ZOOM;
        background.rectTransform.localScale = new Vector3(ZOOM, scaleY, 1f);

        int orient = -backCam.videoRotationAngle;
        background.rectTransform.localEulerAngles = new Vector3(0, 0, orient);
    }

    private void getCameraPermission()
    {
#if PLATFORM_ANDROID
        if (!Permission.HasUserAuthorizedPermission(Permission.Camera))
        {
            Permission.RequestUserPermission(Permission.Camera);
        }
#endif
    }
    private void startCameraDevice()
    {
        getCameraPermission();

        WebCamDevice[] devices = WebCamTexture.devices;
        if (devices.Length == 0)
        {
            Debug.Log("No Camera is available");
            cameraState = CameraDeviceStatus.NotInitialized;
            return;
        }
        for (int i = 0; i < devices.Length; i++)
        {
            if (!devices[i].isFrontFacing)
            {
                int width = Screen.width;
                int height = Screen.height;
                backCam = new WebCamTexture(devices[i].name, width, height);
                Debug.Log(string.Format("camera frame dims: w:{0}, h:{1}", width, height));
                break;
            }
        }

        if (backCam == null)
        {
            Debug.Log("cannot find back camera");
            return;
        }

        cameraState = CameraDeviceStatus.Recording;
        backCam.Play();
        background.texture = backCam;
        Debug.Log(string.Format("backCam: w={0}, h={1}", backCam.width, backCam.height));
        plugin.registerTexture(backCam);
    }

    /// <summary>
    /// Callback for the button controlling camera recording in the Unity scene
    /// </summary>
    public void onCameraButtonClick()
    {
        if (cameraButton == null)
        {
            Debug.Log("Button is NULL");
        }
        shouldStartRecording = !shouldStartRecording;
        string buttonText = shouldStartRecording ? "Stop" : "Start";
        cameraButton.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = buttonText;
        if (!shouldStartRecording && cameraState == CameraDeviceStatus.Recording)
        {
            onRecordingStop();
        }
    }
}
