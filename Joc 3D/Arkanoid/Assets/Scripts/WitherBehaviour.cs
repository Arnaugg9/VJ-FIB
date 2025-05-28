using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class WitherBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;

    //Resources
    private List<AudioClip> hurtClips;
    private List<AudioClip> dieClips;
    private List<AudioClip> idleClips;

    //properties
    public int health;

    //idle sound controll
    public float max_time_idle;
    public float min_time_idle;
    public float idle_timer;

    // Start is called before the first frame update
    void Start()
    {
        health = 5;
        max_time_idle = 35f;
        min_time_idle = 10f;
        idle_timer = Random.Range(min_time_idle, max_time_idle);
        hurtClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Hurt").ToList();
        idleClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Idle").ToList();
        dieClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Die").ToList();
    }

    // Update is called once per frame
    void Update()
    {
        idle_timer -= Time.deltaTime;
        if (idle_timer <= 0)
        {
            int index = Random.Range(0, idleClips.Count);
            AudioSource.PlayClipAtPoint(idleClips[index], Camera.main.transform.position);
            idle_timer = Random.Range(min_time_idle, max_time_idle);
        }
    }

    private void getHurt()
    {
        --health;
        int index = Random.Range(0, idleClips.Count);
        //Die
        if (health <= 0)
        {
            AudioSource.PlayClipAtPoint(idleClips[index], Camera.main.transform.position);
            GameManager.Instance.gameScore += 5000;
            Destroy(gameObject);
        }
        else AudioSource.PlayClipAtPoint(idleClips[index], Camera.main.transform.position);
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Ball")
        {
            getHurt();
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if ((collision.gameObject.tag == "Ball"))
        {
            getHurt();
        }
        else if (collision.gameObject.tag == "Bullet")
        {
            collision.gameObject.GetComponent<BulletBehaviour>().Break();
            getHurt();
        }
    }
}
