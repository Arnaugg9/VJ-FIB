using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockBehaviour : MonoBehaviour
{

    private Rigidbody _rb;
    public bool isGrounded;
    public float fallSpeed;
    public float floorY;
    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
    }

    // Update is called once per frame
    void Update()
    {
        //transform.position = transform.GetChild(0).position;
        isGrounded = Physics.Raycast(transform.position, Vector3.down, out RaycastHit hit, 0.5f);
    }

    private void FixedUpdate()
    {
        if (!isGrounded)
        {
            Vector3 newPos = _rb.position + Vector3.down * fallSpeed * Time.fixedDeltaTime;
            if (newPos.y < floorY) newPos.y = floorY;
            _rb.MovePosition(newPos);
        }
    }

    public void Break()
    {
        Destroy(gameObject);
    }
    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Ball")
        {
            Break();
        }
    }
}
