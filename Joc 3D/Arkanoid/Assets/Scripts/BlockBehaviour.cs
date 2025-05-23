using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class BlockBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;

    //Resources
    public List<AudioClip> breakClips; 

    //triggers
    public bool isGrounded;
    public bool wasDestroyed;

    //properties
    public float fallSpeed;
    public float floorY;

    public int itemSpawnProbability;
    public GameObject itemPrefab;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
        if (tag == "WoodBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Wood").Where(clip => clip.name.StartsWith("Wood_dig")).ToList();
        else if (tag == "GrassBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Grass").Where(clip => clip.name.StartsWith("Grass_dig")).ToList();
        wasDestroyed = false;
        floorY = -0.5f;
        isGrounded = true;
        GameManager.Instance.blocksCurrent++;
        itemSpawnProbability = 7;
    }

    // Update is called once per frame
    void Update()
    {
        //transform.position = transform.GetChild(0).position;
        Vector3 origin = transform.position + Vector3.up * 0.1f; // subir un poco el rayo
        isGrounded = Physics.Raycast(origin, Vector3.down, out RaycastHit hit, 0.11f);
    }

    private void FixedUpdate()
    {
        if (!isGrounded)
        {
            Vector3 newPos = transform.position + Vector3.down * fallSpeed * Time.fixedDeltaTime;
            if (newPos.y < floorY) newPos.y = floorY;
            transform.position = newPos;
        }
    }

    void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Vector3 rayOrigin = transform.position;
        Gizmos.DrawLine(rayOrigin, rayOrigin + Vector3.down * 0.01f);
    }

    public void Break()
    {
        //Spawns item
        int rand = Random.Range(0, 100);
        if (rand < itemSpawnProbability)
        {
            Instantiate(itemPrefab, transform.position, Quaternion.identity);
        }

        //reprodueix audio
        if (breakClips.Count > 0)
        {
            int index = Random.Range(0, breakClips.Count);
            Debug.Log("Reproduciendo sonido: " + breakClips[index].name);
            AudioSource.PlayClipAtPoint(breakClips[index], Camera.main.transform.position);
        }

            wasDestroyed = true;
        GameManager.Instance.blocksDestroyed++;
        GameManager.Instance.gameScore += 500;
        Destroy(gameObject);
    }
    private void OnCollisionEnter(Collision collision)
    {
        if (!wasDestroyed && collision.gameObject.tag == "Ball")
        {
            Break();
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (!wasDestroyed && (collision.gameObject.tag == "Ball"))
        {
            Break();
        }
        else if (!wasDestroyed && collision.gameObject.tag == "Bullet")
        {
            collision.gameObject.GetComponent<BulletBehaviour>().Break();
            Break();
        }
    }
}
