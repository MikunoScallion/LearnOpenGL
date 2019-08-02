using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerVFX : MonoBehaviour
{
    public float speed = 5f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("MoveForward", 0, .1f);
    }

    // Update is called once per frame
    void Update()
    {
        void MoveForward()
        {
            transform.position = Vector3.Slerp(transform.position,
                transform.position + transform.forward * speed, 0.1f);
        }
    }
}
