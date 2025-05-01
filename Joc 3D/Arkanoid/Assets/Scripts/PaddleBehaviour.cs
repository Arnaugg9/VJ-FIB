using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class PaddleBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;

    //Movement
    private Vector3 _dir;
    public float speed;
    public Transform leftWall;
    public Transform rightWall;
    public float wallWidth;

    //Properties
    public float paddleSize;

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
        Vector3 newPos = _rb.position + _dir * speed * Time.fixedDeltaTime;
        if (newPos.x + paddleSize / 2 >= rightWall.position.x - wallWidth/2) newPos.x = rightWall.position.x - wallWidth/2 - paddleSize/2;
        if (newPos.x - paddleSize / 2 <= leftWall.position.x + wallWidth/2) newPos.x = leftWall.position.x + wallWidth/2 + paddleSize / 2;
        _rb.MovePosition(newPos);
    }
}
