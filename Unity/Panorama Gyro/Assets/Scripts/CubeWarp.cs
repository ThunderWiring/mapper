using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeWarp : MonoBehaviour
{
    [SerializeField] private GyroController gyro;
    [SerializeField] private Material material;

    private Texture2D tex;

    // Start is called before the first frame update
    void Start()
    {
        tex = Resources.Load("Textures/panocpy") as Texture2D;
        material.SetTexture("_MainTex", tex);
    }

    // Update is called once per frame
    void Update()
    {
        material.SetMatrix("_WarpMatrix", Matrix4x4.TRS(transform.position, gyro.rot, transform.localScale));
      //  transform.localRotation = gyro.rot;
    }
}
