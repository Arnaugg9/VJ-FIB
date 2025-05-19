using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
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

    public int itemSpawnProbability = 30;
    public GameObject itemPrefab;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
        wasDestroyed = false;
        floorY = -0.5f;
    }

    // Update is called once per frame
    void Update()
    {
        //transform.position = transform.GetChild(0).position;
        isGrounded = Physics.Raycast(transform.position, Vector3.down, out RaycastHit hit, 1f);
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
            int rand = Random.Range(0, 100);
            if (rand < itemSpawnProbability)
            {
                Instantiate(itemPrefab, transform.position, Quaternion.identity);
            }
            if (collision.gameObject.tag == "Bullet")
            {
                collision.gameObject.GetComponent<BulletBehaviour>().Break();
            }
            Break();
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (!wasDestroyed && (collision.gameObject.tag == "Ball" || collision.gameObject.tag == "Bullet"))
        {
            int rand = Random.Range(0, 100);
            if (rand < itemSpawnProbability)
            {
                Instantiate(itemPrefab, transform.position, Quaternion.identity);
            }
            if (collision.gameObject.tag == "Bullet")
            {
                collision.gameObject.GetComponent<BulletBehaviour>().Break();
            }
            Break();
        }
    }
}
