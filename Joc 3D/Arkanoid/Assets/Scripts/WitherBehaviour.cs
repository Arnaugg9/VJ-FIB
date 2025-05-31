using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class WitherBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;
    private Animator _animator;
    public WitherSpawn spawner;

    //Resources
    private List<AudioClip> hurtClips;
    private List<AudioClip> dieClips;
    private List<AudioClip> idleClips;
    private AudioClip shootClip;
    private AudioClip spawnClìp;
    public GameObject hitParticles;
    public GameObject shootParticles;
    public GameObject deathParticles;

    //properties
    private int max_health;
    public int health;

    //idle sound controll
    public float max_time_idle;
    public float min_time_idle;
    public float idle_timer;

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
    private bool isMoving = true;

    private float random_stop_min;
    private float random_stop_max;
    public float next_random_stop;
    private float random_stop_duration;

    //Shoot
    public GameObject normalSkullPrefab;
    public GameObject chargedSkullPrefab;
    public Transform shootPoint;
    public float min_time_shoot;
    public float max_time_shoot;
    private float shootTimer;

    //ChangeDir
    public float stopDuration = 1f;
    private float stopTimer = 0f;

    private float totalInvincivility;
    private float currentInvencivilityPassed;
    private float damageAnimationTime;
    private float currentdamageAimationTime;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _animator = GetComponent<Animator>();

        max_health = 7;
        health = max_health;
        UIBehaviour.Instance.drawBossUI(health, max_health);
        max_time_idle = 25f;
        min_time_idle = 10f;
        idle_timer = Random.Range(min_time_idle, max_time_idle);

        speed = 6f;
        currentDirection = directions[Random.Range(0, directions.Length)];
        isMoving = true;

        random_stop_max = 13f;
        random_stop_min = 5f;
        next_random_stop = Random.Range(random_stop_min, random_stop_max);
        random_stop_duration = 3f;

        min_time_shoot = 4f;
        max_time_shoot = 10f;
        shootTimer = Random.Range(min_time_shoot, max_time_shoot);

        totalInvincivility = 1.5f;
        currentInvencivilityPassed = 0;
        damageAnimationTime = 0.2f;
        currentdamageAimationTime = 0;

        hurtClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Hurt").ToList();
        idleClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Idle").ToList();
        dieClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Wither/Die").ToList();
        shootClip = Resources.Load<AudioClip>("Audio/Mobs/Wither/Other/Wither_shoot");
        spawnClìp = Resources.Load<AudioClip>("Audio/Mobs/Wither/Other/Wither_spawn");
        AudioSource.PlayClipAtPoint(spawnClìp, Camera.main.transform.position);
    }

    // Update is called once per frame
    void Update()
    {
        //Control idle sounds
        idle_timer -= Time.deltaTime;
        if (idle_timer <= 0)
        {
            int index = Random.Range(0, idleClips.Count);
            AudioSource.PlayClipAtPoint(idleClips[index], Camera.main.transform.position);
            idle_timer = Random.Range(min_time_idle, max_time_idle);
        }

        //Control moviment
        if (isMoving)
        {
            _rb.MovePosition(transform.position + currentDirection * speed * Time.deltaTime);
            next_random_stop -= Time.deltaTime;
            if (next_random_stop <= 0)
            {
                isMoving = false;
                stopTimer = random_stop_duration;
                next_random_stop = Random.Range(random_stop_min, random_stop_max);
            }
        }
        else
        {
            stopTimer -= Time.deltaTime;
            if (stopTimer <= 0)
            {
                isMoving = true;
                currentDirection = directions[Random.Range(0, directions.Length)];
            }
        }

        //Control shoot
        shootTimer -= Time.deltaTime;
        if (shootTimer <= 0)
        {
            StartCoroutine(Shoot());
            shootTimer = Random.Range(min_time_shoot, max_time_shoot);
        }

        //Control invencibilitat
        if (currentInvencivilityPassed > 0)
        {
            currentInvencivilityPassed -= Time.deltaTime;
            currentdamageAimationTime -= Time.deltaTime;
            if (currentdamageAimationTime <= 0)
            {
                currentdamageAimationTime = damageAnimationTime;
                GameObject go = gameObject.transform.GetChild(0).gameObject;
                go.SetActive(!go.activeSelf);
            }
            if (currentInvencivilityPassed <= 0) gameObject.transform.GetChild(0).gameObject.SetActive(true);
        }
    }

    private IEnumerator Shoot()
    {
        isMoving = false;
        stopTimer = stopDuration;

        yield return new WaitForSeconds(0.2f);

        GameObject particle;
        if (shootParticles != null) particle = Instantiate(shootParticles, shootPoint.position, Quaternion.identity);
        AudioSource.PlayClipAtPoint(shootClip, Camera.main.transform.position);

        GameObject bullet = Instantiate((health < 3 ? chargedSkullPrefab : normalSkullPrefab), shootPoint.position, Quaternion.identity);
        Vector3 direction = GameManager.Instance.paddle.transform.position - shootPoint.position;
        bullet.GetComponent<EnemyBulletBehaviour>().SetDirection(direction);

        // Esperar el tiempo de parada antes de volver a moverse
        yield return new WaitForSeconds(0.3f);

        currentDirection = directions[Random.Range(0, directions.Length)];
        isMoving = true;
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
            // Queremos que la nueva dirección se aleje en el eje X si colisionó en X, y lo mismo en Z
            bool awayInX = Mathf.Sign(dir.x) != Mathf.Sign(toObstacle.x) || Mathf.Approximately(toObstacle.x, 0);
            bool awayInZ = Mathf.Sign(dir.z) != Mathf.Sign(toObstacle.z) || Mathf.Approximately(toObstacle.z, 0);

            if (awayInX && awayInZ)
            {
                validDirs.Add(dir);
            }
        }

        // Si no se encontró ninguna (puede pasar con algunas colisiones), devolver una distinta a la actual
        if (validDirs.Count == 0)
        {
            return GetRandomDirection();
        }

        return validDirs[Random.Range(0, validDirs.Count)];
    }

    private void getHurt()
    {
        if (currentInvencivilityPassed <= 0)
        {
            currentInvencivilityPassed = totalInvincivility;
            currentdamageAimationTime = damageAnimationTime;
            gameObject.transform.GetChild(0).gameObject.SetActive(false);
            --health;
            UIBehaviour.Instance.drawBossUI(health, max_health);
            //Die
            if (health <= 0)
            {
                int index = Random.Range(0, dieClips.Count);
                AudioSource.PlayClipAtPoint(dieClips[index], Camera.main.transform.position);
                GameObject particle;
                if (deathParticles != null) particle = Instantiate(deathParticles, transform.position, Quaternion.identity);
                GameManager.Instance.gameScore += 5000;
                spawner.isDead = true;
                Destroy(gameObject);
            }
            else
            {
                int index = Random.Range(0, hurtClips.Count);
                AudioSource.PlayClipAtPoint(hurtClips[index], Camera.main.transform.position);
                GameObject particle;
                if (hitParticles != null) particle = Instantiate(hitParticles, transform.position, Quaternion.identity);
            }
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        //Debug.Log("Collided with " + collision.gameObject.tag);
        if (collision.gameObject.CompareTag("MobBarrier"))
        {
            // Solo escoger direcciones que vayan hacia atrás (Z negativo o cero)
            List<Vector3> backwardDirs = directions.Where(dir => dir.z > 0).ToList();
            currentDirection = backwardDirs[Random.Range(0, backwardDirs.Count)];

        }
        else if (collision.gameObject.layer == LayerMask.NameToLayer("Block") || collision.gameObject.tag == "Walls")
        {
            currentDirection = GetEscapeDirection(collision.transform.position);

        }
        else if ((collision.gameObject.tag == "Ball"))
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
