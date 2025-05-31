using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using static UnityEngine.Rendering.VirtualTexturing.Debugging;

public class DragonBullet : MonoBehaviour
{
    public float speed = 10f;
    public float endZ;
    public GameObject poisonPrefab;
    public GameObject explosionParticle;

    public List<AudioClip> explosionClips;

    public bool isLast = false;

    private void Start()
    {
        endZ = GameManager.Instance.paddle.gameObject.transform.position.z - 0.05f;
        explosionClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Explosion").ToList();
    }

    private void Update()
    {
        // Mover hacia adelante (en la dirección local "forward" del proyectil)
        transform.Translate(Vector3.forward * speed * Time.deltaTime, Space.Self);
        if (isLast)
        {
            if (transform.position.z <= endZ)
            {
                Instantiate(poisonPrefab, transform.position, Quaternion.identity);
                Break();
            }
        }
    }

    private void Break()
    {
        int index = Random.Range(0, explosionClips.Count);
        AudioSource.PlayClipAtPoint(explosionClips[index], Camera.main.transform.position, 80);
        Instantiate(explosionParticle, transform.position, Quaternion.identity);
        Destroy(gameObject);
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Paddle"))
        {
            // Llamar al método getHurt del paddle
            GameManager.Instance.loseLife();

            // Destruir la bala tras colisión
            if (isLast) Instantiate(poisonPrefab, transform.position, Quaternion.identity);
            Destroy(gameObject);
        }
        else if (other.CompareTag("Walls"))
        {
            Break();
        }
        else if (other.CompareTag("DeathZone"))
        {
            Break();
        }
    }
}
