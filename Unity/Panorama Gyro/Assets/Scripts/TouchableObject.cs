using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class TouchableObject : MonoBehaviour
{
    [SerializeField] private Camera camera;

    private Vector3 previousPosition;

    abstract protected void onEscape();
    protected void onFrameUpdate() { }

    private void moveOnTouch()
    {
        if (Input.touchCount == 0)
        {
            return;
        }

        Touch screenTouch = Input.GetTouch(0);
        if (screenTouch.phase == TouchPhase.Moved)
        {
            Vector3 direction = previousPosition - camera.ScreenToViewportPoint(screenTouch.position);
            camera.transform.position = previousPosition;
            camera.transform.Rotate(new Vector3(1, 0, 0), direction.y * 180);
            camera.transform.Rotate(new Vector3(0, 1, 0), -direction.x * 180, Space.World);
            //camera.transform.Translate(new Vector3(0, 0, 0));
            previousPosition = camera.ScreenToViewportPoint(screenTouch.position);
        }
        else if (screenTouch.phase == TouchPhase.Began)
        {
            previousPosition = camera.ScreenToViewportPoint(screenTouch.position);
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Escape))
        {
            onEscape();
            return;
        }
        moveOnTouch();
        onFrameUpdate();
    }
}
