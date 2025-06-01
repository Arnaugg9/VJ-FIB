using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class BulletBehaviour : MonoBehaviour
{
    private Rigidbody _rb;

    public int speed;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void FixedUpdate()
    {
        Vector3 newPos = _rb.position + Vector3.forward * speed * Time.fixedDeltaTime;
        _rb.MovePosition(newPos);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Walls")
        {
            Break();
        }
    }

    public void Break()
    {
        Destroy(gameObject);
    }
}
