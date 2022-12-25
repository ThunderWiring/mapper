using UnityEngine;
using UnityEngine.Android;
using UnityEngine.SceneManagement;
using UnityEngine.UI;


enum CameraDeviceStatus
{
    NotInitialized, Recording, Paused
}

enum CaptureMode { 
    Video, Snapshot
}

/// <summary>
/// Controls the device camera and reads frames from the main camera for processing.
/// </summary>
[RequireComponent(typeof(GyroController), typeof(NativePlugin))]
public class CameraDevice : MonoBehaviour
{
    [SerializeField] private Toggle videoCamToggle; /* Used to switch between video and single shots mode. */
    [SerializeField] private Button cameraButton; /* Triggeres taking a snapshot or video */
    [SerializeField] private RawImage background; /* Canvas displaying the camera feed. */
    [SerializeField] private AspectRatioFitter aspectRatioFitter;

    private const float ZOOM = 0.43f;
    private const int MIN_WIDTH_THRESH = 100; // used to indicate whether the camera frame is being received whole.

    private bool shouldStartRecording = false;
    private CameraDeviceStatus cameraState = CameraDeviceStatus.NotInitialized;
    private WebCamTexture backCam;
    private GyroController gyroController;
    private NativePlugin plugin;
    private CaptureMode captureMode;

    private void Awake()
    {
        Screen.orientation = ScreenOrientation.Portrait;
        gyroController = GetComponent<GyroController>();
        plugin = GetComponent<NativePlugin>();
    }

    void Update()
    {
        captureMode = videoCamToggle.isOn ? CaptureMode.Video : CaptureMode.Snapshot;
        string cameraButtonText = captureMode == CaptureMode.Snapshot 
            ? "Take Image" 
            : shouldStartRecording 
                ? "Stop" : "Start";
        cameraButton.GetComponentInChildren<TMPro.TextMeshProUGUI>().text = cameraButtonText;

        if (captureMode == CaptureMode.Snapshot) {
            shouldStartRecording = false;
        } else if (shouldStartRecording) {
            //passFrameToPlugin();
        }
        updateCameraFrame();
    }

    private void passFrameToPlugin() {
        plugin.sendCameraFramesToPlugin(
            backCam.GetPixels32(), backCam.width, backCam.height, gyroController.rot);
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
            UnityLogger.DLog(
                "Still is waiting another frame for correct info. Frame dims: ({0}, {1}", backCam.width, backCam.height
            );
            return;
        }
        setFrameAspectRatio();        
    }

    private void setFrameAspectRatio()
    {
        float ratio = (float)backCam.width / (float)backCam.height;
        aspectRatioFitter.aspectRatio = ratio;
        float scaleY = backCam.videoVerticallyMirrored ? -ZOOM : ZOOM;
        background.rectTransform.localScale = new Vector3(ZOOM, scaleY, 1f);

        int orient = -backCam.videoRotationAngle;
        background.rectTransform.localEulerAngles = new Vector3(0, 0, orient);
        UnityLogger.DLog("CamDevice:aspectRatio - w:{0} h:{1}, ratio:{2}",
            backCam.width, backCam.height, ratio);
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
            return;
        }
        else if (captureMode == CaptureMode.Snapshot) {
            passFrameToPlugin();
            return;
        }

        shouldStartRecording = !shouldStartRecording;
        if (!shouldStartRecording && cameraState == CameraDeviceStatus.Recording)
        {
            onRecordingStop();
        }
    }
}
