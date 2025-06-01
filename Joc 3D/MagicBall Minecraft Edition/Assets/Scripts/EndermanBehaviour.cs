using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class EndermanBehaviour : MonoBehaviour
{
    public float minTime = 4f;
    public float maxTime = 12f;

    public float left;
    public float right;
    public float bottom;
    public float top;

    public Transform leftWall;
    public Transform rightWall;
    public Transform topWall;
    public Transform bottomWall;

    private float teleportTimer;

    private BoxCollider col;
    private Rigidbody _rb;

    public List<AudioClip> teleportClips;

    // Movement
    private Vector3[] directions = new Vector3[]
    {
        Vector3.right,
        Vector3.left,
        Vector3.forward,
        Vector3.back,
        (Vector3.right + Vector3.forward).normalized,
        (Vector3.right + Vector3.back).normalized,
        (Vector3.left + Vector3.forward).normalized,
        (Vector3.left + Vector3.back).normalized
    };
    public float speed = 2.5f;
    private Vector3 currentDirection;

    // Random direction change
    private float random_stop_min = 10f;
    private float random_stop_max = 20f;
    private float next_random_stop;

    void Start()
    {
        col = GetComponent<BoxCollider>();
        _rb = GetComponent<Rigidbody>();

        teleportClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Enderman/Teleport").ToList();

        left = leftWall.position.x;
        right = rightWall.position.x;
        bottom = bottomWall.position.z;
        top = topWall.position.z;

        teleportTimer = Random.Range(minTime, maxTime);
        currentDirection = directions[Random.Range(0, directions.Length)];
        next_random_stop = Random.Range(random_stop_min, random_stop_max);
    }

    void Update()
    {
        // Movimiento
        Vector3 targetPosition = transform.position + currentDirection * speed * Time.deltaTime;
        if (IsValidPosition(targetPosition))
        {
            _rb.MovePosition(targetPosition);
        }
        else
        {
            currentDirection = GetEscapeDirection(targetPosition);
        }

        if (currentDirection != Vector3.zero)
        {
            Quaternion targetRotation = Quaternion.LookRotation(currentDirection, Vector3.up);
            Quaternion correction = Quaternion.Euler(0, 0, 0);
            transform.GetChild(0).rotation = Quaternion.Slerp(transform.GetChild(0).rotation, targetRotation * correction, 10f * Time.deltaTime);
        }

        // Cambio aleatorio de dirección
        next_random_stop -= Time.deltaTime;
        if (next_random_stop <= 0)
        {
            currentDirection = GetRandomDirection();
            next_random_stop = Random.Range(random_stop_min, random_stop_max);
        }

        // Teletransporte
        teleportTimer -= Time.deltaTime;
        if (teleportTimer <= 0f)
        {
            TryTeleport();
            teleportTimer = Random.Range(minTime, maxTime);
        }
    }

    void TryTeleport()
    {
        for (int attempts = 0; attempts < 20; attempts++)
        {
            Vector3 newPos = GetRandomPositionWithinBounds();
            if (IsValidPosition(newPos))
            {
                transform.position = newPos;
                int rand = Random.Range(0, teleportClips.Count);
                AudioSource.PlayClipAtPoint(teleportClips[rand], Camera.main.transform.position);
                break;
            }
        }
    }

    Vector3 GetRandomPositionWithinBounds()
    {
        float newX = Random.Range(left, right);
        float newZ = Random.Range(bottom, top);
        float y = transform.position.y;
        return new Vector3(newX, y, newZ);
    }

    bool IsValidPosition(Vector3 position)
    {
        Vector3 halfExtents = col.size / 2f;
        Vector3 checkCenter = position + col.center;

        Collider[] hits = Physics.OverlapBox(
            checkCenter,
            halfExtents,
            transform.rotation
        );

        foreach (Collider hit in hits)
        {
            if (hit.gameObject == gameObject) continue;

            if (hit.CompareTag("Walls") ||
                hit.CompareTag("MobBarrier") ||
                hit.gameObject.layer == LayerMask.NameToLayer("Block"))
            {
                return false;
            }
        }

        return true;
    }

    private Vector3 GetRandomDirection()
    {
        Vector3 newDir;
        do
        {
            newDir = directions[Random.Range(0, directions.Length)];
        } while (newDir == currentDirection);
        return newDir;
    }

    private Vector3 GetEscapeDirection(Vector3 obstaclePosition)
    {
        Vector3 toObstacle = obstaclePosition - transform.position;
        List<Vector3> validDirs = new List<Vector3>();

        foreach (var dir in directions)
        {
            bool awayInX = Mathf.Sign(dir.x) != Mathf.Sign(toObstacle.x) || Mathf.Approximately(toObstacle.x, 0);
            bool awayInZ = Mathf.Sign(dir.z) != Mathf.Sign(toObstacle.z) || Mathf.Approximately(toObstacle.z, 0);

            if (awayInX && awayInZ)
            {
                validDirs.Add(dir);
            }
        }

        if (validDirs.Count == 0)
        {
            return GetRandomDirection();
        }

        return validDirs[Random.Range(0, validDirs.Count)];
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.CompareTag("MobBarrier"))
        {
            List<Vector3> backwardDirs = directions.Where(dir => dir.z > 0).ToList();
            currentDirection = backwardDirs[Random.Range(0, backwardDirs.Count)];
        }
        else if (collision.gameObject.layer == LayerMask.NameToLayer("Block") || collision.gameObject.tag == "Walls")
        {
            currentDirection = GetEscapeDirection(collision.transform.position);
        }
    }
}
