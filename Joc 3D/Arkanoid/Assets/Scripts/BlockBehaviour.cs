using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;
    //triggers
    public bool isGrounded;
    public bool wasDestroyed;

    //properties
    public float fallSpeed;
    public float floorY;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
        wasDestroyed = false;
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
        wasDestroyed = true;
        GameManager.Instance.blocksDestroyed++;
        GameManager.Instance.gameScore += 500;
        Destroy(gameObject);
    }
    private void OnCollisionEnter(Collision collision)
    {
        if (!wasDestroyed && collision.gameObject.tag == "Ball")
        {
            if (tag == "SmallPU")
            {
                GameManager.Instance.changePaddleSize(1.75f);
            }
            else if (tag == "BigPU")
            {
                GameManager.Instance.changePaddleSize(3.75f);
            }
            else if (tag == "PowerPU")
            {
                collision.collider.GetComponent<BallBehaviour>().setPower(true);
            }
            else if (tag == "MagnetPU")
            {
                GameManager.Instance.paddle.activateMagnet();
            }
            else if (tag == "ClonePU")
            {
                GameManager.Instance.cloneBall(collision.collider.transform.position);
            }
            
            Break();
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (!wasDestroyed && collision.gameObject.tag == "Ball")
        {
            if (tag == "SmallPU")
            {
                GameManager.Instance.changePaddleSize(1.75f);
            }
            else if (tag == "BigPU")
            {
                GameManager.Instance.changePaddleSize(3.75f);
            }
            else if (tag == "PowerPU")
            {
                collision.GetComponent<BallBehaviour>().setPower(true);
            }
            else if (tag == "MagnetPU")
            {
                GameManager.Instance.paddle.activateMagnet();
            }
            else if (tag == "ClonePU")
            {
                GameManager.Instance.cloneBall(collision.transform.position);
            }
            Break();
        }
    }
}
