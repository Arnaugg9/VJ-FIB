using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DragonSpawner : MonoBehaviour
{
    public bool started;
    public bool isDead;
    public GameObject dragon;

    // Start is called before the first frame update
    void Start()
    {
        started = false;
        isDead = false;
        GameManager.Instance.dragonSpawner = this;
    }

    public void startFight()
    {
        started = true;
        dragon.GetComponent<DragonBehaviour>().startFight(transform.position);
    }

    // Update is called once per frame
    void Update()
    {
        if (started) isDead = (dragon == null);
    }
}
