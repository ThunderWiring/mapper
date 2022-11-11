using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnityLogger
{
    public static void DLog(string msg, params object[] args) {
        Debug.Log(string.Format(msg, args));
    }
}
