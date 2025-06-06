using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class ZombieBehaviour : MonoBehaviour
{
    // Start is called before the first frame update
    private Rigidbody _rb;

    //Movement
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
    public float speed;
    private Vector3 currentDirection;

    //Random change dir
    private float random_stop_min;
    private float random_stop_max;
    public float next_random_stop;

    //posControll
    private BoxCollider col;

    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        col = GetComponent<BoxCollider>();
        speed = 0.75f;
        currentDirection = directions[Random.Range(0, directions.Length)];

        random_stop_max = 20f;
        random_stop_min = 10f;
        next_random_stop = Random.Range(random_stop_min, random_stop_max);

    }

    // Update is called once per frame
    void Update()
    {
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

        //Canvio direccion aleatorio
        next_random_stop -= Time.deltaTime;
        if (next_random_stop <= 0)
        {
            currentDirection = directions[Random.Range(0, directions.Length)];
            next_random_stop = Random.Range(random_stop_min, random_stop_max);
        }
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
            // Queremos que la nueva direcci�n se aleje en el eje X si colision� en X, y lo mismo en Z
            bool awayInX = Mathf.Sign(dir.x) != Mathf.Sign(toObstacle.x) || Mathf.Approximately(toObstacle.x, 0);
            bool awayInZ = Mathf.Sign(dir.z) != Mathf.Sign(toObstacle.z) || Mathf.Approximately(toObstacle.z, 0);

            if (awayInX && awayInZ)
            {
                validDirs.Add(dir);
            }
        }

        // Si no se encontr� ninguna (puede pasar con algunas colisiones), devolver una distinta a la actual
        if (validDirs.Count == 0)
        {
            return GetRandomDirection();
        }

        return validDirs[Random.Range(0, validDirs.Count)];
    }

    private void OnTriggerEnter(Collider collision)
    {
        //Debug.Log("Collided with " + collision.gameObject.tag);
        if (collision.gameObject.CompareTag("MobBarrier"))
        {
            // Solo escoger direcciones que vayan hacia atr�s (Z negativo o cero)
            List<Vector3> backwardDirs = directions.Where(dir => dir.z > 0).ToList();
            currentDirection = backwardDirs[Random.Range(0, backwardDirs.Count)];

        }
        else if (collision.gameObject.layer == LayerMask.NameToLayer("Block") || collision.gameObject.tag == "Walls")
        {
            currentDirection = GetEscapeDirection(collision.transform.position);
        }
    }
}
