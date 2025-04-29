using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class ShipController : MonoBehaviour
{
    //Components
    private Rigidbody _rb;

    //Movement
    private Vector3 _dir;
    public float speed;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _dir = Vector3.zero;
    }

    // Update is called once per frame
    void Update()
    {
        _dir = Vector3.zero;
        if (Input.GetKey(KeyCode.D) ||Input.GetKey(KeyCode.RightArrow))
        {
            _dir = Vector3.right;
        }
        if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
        {
            _dir = Vector3.left;
        }


    }

    private void FixedUpdate()
    {
        _rb.velocity = _dir * speed;
    }
}
