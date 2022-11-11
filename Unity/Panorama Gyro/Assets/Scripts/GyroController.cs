using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GyroController : MonoBehaviour
{
    private bool isGyroEnabled;
    private Gyroscope gyro;
    private Quaternion rotation;


    void Start()
    {
        Debug.Log("Start - Enabling Gyroscope...");
        isGyroEnabled = enableGyro();
    }

    
    void Update()
    {
        if (!isGyroEnabled) {
            return;
        }
        rotation = gyro.attitude;
    }
    
    public Quaternion rot {
        get => rotation; 
    }

    private bool enableGyro()
    {
        if (!SystemInfo.supportsGyroscope)
        {
            Debug.Log("Gyroscope is not supported on this device.");
            return false;
        }
        gyro = Input.gyro;
        gyro.enabled = true;
        Debug.Log("Gyro enabled");
        return true;
    }
}
