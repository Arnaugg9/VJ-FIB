using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WitherSpawn : MonoBehaviour
{
    public bool hasSpawned;
    public bool isDead;
    public GameObject witherPrefab;

    // Start is called before the first frame update
    void Start()
    {
        hasSpawned = false;
        isDead = false;
        GameManager.Instance.witherSpawner = this;
    }

    public void spawnWither()
    {
        hasSpawned = true;
        GameObject wither = Instantiate(witherPrefab, transform.position, Quaternion.identity);
        wither.GetComponent<WitherBehaviour>().spawner = this;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
