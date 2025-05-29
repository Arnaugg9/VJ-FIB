using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class EnemyBulletBehaviour : MonoBehaviour
{
    private Rigidbody _rb;
    private Vector3 _dir;

    private List<AudioClip> explosionClips;
    public GameObject explosionParticles;

    public int speed = 10;

    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        explosionClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Explosion").ToList();
    }

    public void SetDirection(Vector3 dir)
    {
        _dir = dir.normalized;
        //Fa que la bal s'orienti cap a on va
        transform.forward = _dir;
    }

    private void FixedUpdate()
    {
        Vector3 newPos = _rb.position + _dir * speed * Time.fixedDeltaTime;
        _rb.MovePosition(newPos);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Walls")
        {
            Break();
        }
        else if (collision.gameObject.tag == "DeathZone")
        {
            Break();
        }
        else if (collision.gameObject.tag == "Paddle")
        {
            GameManager.Instance.loseLife();
            Break();
        }
    }

    public void Break()
    {
        int index = Random.Range(0, explosionClips.Count);
        AudioSource.PlayClipAtPoint(explosionClips[index], Camera.main.transform.position);

        GameObject particle;
        if (explosionParticles != null) particle = Instantiate(explosionParticles, transform.position, Quaternion.identity);

        Destroy(gameObject);
    }

}
