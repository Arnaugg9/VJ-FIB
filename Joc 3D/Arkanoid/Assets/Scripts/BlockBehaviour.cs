using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using static UnityEngine.ParticleSystem;

public class BlockBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;

    //Resources
    private List<AudioClip> breakClips;
    private List<AudioClip> breakClips2;

    public GameObject destroyParticles;

    //triggers
    public bool isGrounded;
    public bool wasDestroyed;

    //properties
    public float fallSpeed;
    public float floorY;

    //Dissenys per fer randomitzacio de blocks en lvl3
    public List<GameObject> oreBlocks;

    public int itemSpawnProbability;
    public GameObject itemPrefab;

    //idle sound control for mobs
    bool hasIdle;
    public float max_time_idle;
    public float min_time_idle;
    public float idle_timer;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
        max_time_idle = 20f;
        min_time_idle = 5f;
        idle_timer = Random.Range(min_time_idle, max_time_idle);
        hasIdle = false;
        assignSounds();
        if (tag == "StoneBlock") changeRandomly();
        wasDestroyed = false;
        floorY = -0.5f;
        isGrounded = true;
        if (tag != "BarrierBlock") GameManager.Instance.blocksCurrent++;
        itemSpawnProbability = GameManager.Instance.itemSpawnProbability;
    }

    private void assignSounds()
    {
        if (tag == "WoodBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Wood").ToList();
        else if (tag == "GrassBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Grass").ToList();
        else if (tag == "Villager")
        {
            hasIdle = true;
            breakClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Villager/Die").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Mobs/Villager/Idle").ToList();
        }
        else if (tag == "Fox")
        {
            hasIdle = true;
            breakClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Fox/Die").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Mobs/Fox/Idle").ToList();
        }
        else if (tag == "CactusBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Cactus").ToList();
        else if (tag == "SandBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Sand").ToList();
        else if (tag == "Golem") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Golem").ToList();
        else if (tag == "StoneBlock" || tag == "NoStoneBlock" || tag == "BedrockBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Stone").ToList();
        else if (tag == "MinecartBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Minecart").ToList();
        else if (tag == "Zombie")
        {
            hasIdle = true;
            breakClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Zombie/Die").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Mobs/Zombie/Idle").ToList();
        }
        else if (tag == "CrystalBlock" || tag == "EndPortalBlock" || tag == "BarrierBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Crystal").ToList();
        else if (tag == "NetherBrickBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/NetherBrick").ToList();
        else if (tag == "NetherrackBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Netherrack").ToList();
        else if (tag == "SoulSandBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/SoulSand").ToList();
        else if (tag == "EndCrystalBlock")
        {
            breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Crystal").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Blocks/Explosion").ToList();
        }
        else if (tag == "Enderman")
        {
            hasIdle = true;
            breakClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Enderman/Die").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Mobs/Enderman/Idle").ToList();
        }
    }

    private void changeRandomly()
    {
        int rand = Random.Range(0, 100);
        if (rand < 35)
        {
            rand = Random.Range(0, 5 + 4 + 2 + 1);
            Destroy(transform.GetChild(0).gameObject);
            GameObject newBlock;
            if (rand < 5) newBlock = oreBlocks[0];
            else if (rand < 9) newBlock = oreBlocks[1];
            else if (rand < 11) newBlock = oreBlocks[2];
            else newBlock = oreBlocks[3];

            newBlock = Instantiate(newBlock, transform);
            newBlock.transform.localPosition = Vector3.zero;
            newBlock.transform.localScale = new Vector3(0.5f, 0.5f, 0.5f);
        }
    }

    // Update is called once per frame
    void Update()
    {
        //transform.position = transform.GetChild(0).position;
        Vector3 origin = transform.position + Vector3.up * 0.1f; // subir un poco el rayo
        isGrounded = Physics.Raycast(origin, Vector3.down, out RaycastHit hit, 0.11f);

        if (hasIdle)
        {
            idle_timer -= Time.deltaTime;
            if (idle_timer <= 0)
            {
                int index = Random.Range(0, breakClips2.Count);
                AudioSource.PlayClipAtPoint(breakClips2[index], Camera.main.transform.position);
                idle_timer = Random.Range(min_time_idle, max_time_idle);
            }
        }
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
        if (tag != "BarrierBlock")
        {
            int rand = Random.Range(0, 100);
            if (rand < itemSpawnProbability)
            {
                Instantiate(itemPrefab, transform.position + new Vector3(0, 0.8f, 0), Quaternion.identity);
            }
        }

        //reprodueix audio
        if (breakClips.Count > 0)
        {
            int index = Random.Range(0, breakClips.Count);
            AudioSource.PlayClipAtPoint(breakClips[index], Camera.main.transform.position);
            if (tag == "EndCrystalBlock" && breakClips2.Count > 0)
            {
                index = Random.Range(0, breakClips2.Count);
                AudioSource.PlayClipAtPoint(breakClips2[index], Camera.main.transform.position);
            }
        }

        //Genera Particules
        GameObject particle;
        if (destroyParticles != null) particle = Instantiate(destroyParticles, transform.position, Quaternion.identity);

        wasDestroyed = true;
        if (tag != "BarrierBlock")
        {
            GameManager.Instance.blocksDestroyed++;
            GameManager.Instance.gameScore += 500;
        }
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
