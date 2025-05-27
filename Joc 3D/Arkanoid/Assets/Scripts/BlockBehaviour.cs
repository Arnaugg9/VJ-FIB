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
    public bool isDestroyable;

    //properties
    public float fallSpeed;
    public float floorY;

    //Dissenys per fer randomitzacio de blocks en lvl3
    public List<GameObject> oreBlocks;

    public int itemSpawnProbability;
    public GameObject itemPrefab;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true;
        assignSounds();
        if (tag == "StoneBlock") changeRandomly();

        wasDestroyed = false;
        floorY = -0.5f;
        isGrounded = true;
        isDestroyable = false;

        if (tag == "EndPortalBlock")
        {
            gameObject.SetActive(false);
            GameManager.Instance.portalBlocks.Add(gameObject);
            GameManager.Instance.nPortalBlocks++;
        }
        else if (tag == "DragonEgg")
        {
            gameObject.SetActive(false);
            GameManager.Instance.DragonEgg = gameObject;
        }
        else if (tag == "BedrockBlock")
        {
            GameManager.Instance.portalBlocks.Add(gameObject);
            GameManager.Instance.nPortalBlocks++;
        }
        else
        {
            GameManager.Instance.blocksCurrent++;
            isDestroyable = true;
        }

        itemSpawnProbability = 15;
    }

    private void assignSounds()
    {
        if (tag == "WoodBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Wood").ToList();
        else if (tag == "GrassBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Grass").ToList();
        else if (tag == "CactusBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Cactus").ToList();
        else if (tag == "SandBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Sand").ToList();
        else if (tag == "StoneBlock" || tag == "NoStoneBlock" || tag == "BedrockBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Stone").ToList();
        else if (tag == "CrystalBlock" || tag == "EndPortalBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Crystal").ToList();
        else if (tag == "NetherBrickBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/NetherBrick").ToList();
        else if (tag == "NetherrackBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Netherrack").ToList();
        else if (tag == "SoulSandBlock") breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/SoulSand").ToList();
        else if (tag == "EndCrystalBlock")
        {
            breakClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Crystal").ToList();
            breakClips2 = Resources.LoadAll<AudioClip>("Audio/Blocks/Explosion").ToList();
        }
        else breakClips = new List<AudioClip>();
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
        if (isDestroyable)
        {
            //Spawns item
            int rand = Random.Range(0, 100);
            if (rand < itemSpawnProbability)
            {
                Instantiate(itemPrefab, transform.position + new Vector3(0, 0.8f, 0), Quaternion.identity);
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
            GameManager.Instance.blocksDestroyed++;
            GameManager.Instance.gameScore += 500;
            if (tag == "EndPortalBlock" || tag == "BedrockBlock") GameManager.Instance.nPortalBlocks--;
            else if (tag == "DragonEgg") GameManager.Instance.endGame();
            Destroy(gameObject);
        }
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
