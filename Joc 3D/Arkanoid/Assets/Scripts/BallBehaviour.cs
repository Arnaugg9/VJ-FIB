using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallBehaviour : MonoBehaviour
{
    //components
    private Rigidbody _rb;
    public GameObject ship;

    //Movement controll
    public int speed;
    private Vector3 _dir;

    //triggers
    private bool _wasShoot;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true; 
        _wasShoot = false;
        _dir = Vector3.zero;
    }

    // Update is called once per frame
    void Update()
    {
        if (!_wasShoot && Input.GetKey(KeyCode.Space))
        {
            _wasShoot = true;
            _rb.isKinematic = false;
            _dir = new Vector3(Random.Range(-0.75f, 0.75f), 0, 1);
            _rb.velocity = _dir * speed;
        }
    }

    private void FixedUpdate()
    {
        if (!_wasShoot)
        {
            transform.position = ship.transform.position + new Vector3(0, 0.5f, 0.5f);
        }
    }
}
