using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

/// <summary>
/// Displays the Frame Stats and info on the canvas
/// </summary>
[RequireComponent(typeof(GyroController))]
public class FrameInfo : MonoBehaviour
{
    [SerializeField] private TextMeshProUGUI text;
    private GyroController gyro;

    void Awake()
    {
        gyro = GetComponent<GyroController>();
    }


    void Update()
    {
        text.text = getFrameInfo();
    }

    private string getFrameInfo()
    {
        return string.Format(
            "Euler Angles: (xy, xz, yz - portrait2landscape)) {0}\nQ: {1}",
            gyro.rot.eulerAngles, gyro.rot
       );
    }
}
