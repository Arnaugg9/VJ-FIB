using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using UnityEngine;
using UnityEngine.UIElements;

public class DragonBoss : MonoBehaviour
{
    // Tiempo de espera entre ataques
    private float min_time_shoot = 2f;
    private float max_time_shoot = 5f;
    public float shootTimer;

    // Tiempo de vuelo después de disparar
    private float min_time_fly = 4f;
    private float max_time_fly = 8f;
    public float flyTimer;

    private float targetFlyHeight = 8f;   
    private float flyAscendSpeed = 8f;     
    public bool ascending = true;
    public Transform basePoint;
    private float baseY;
    public Transform fallShootPoint;

    // Límites del área de vuelo
    private float left;
    private float right;
    private float bottom;
    private float top;
    public Transform leftWall;
    public Transform rightWall;
    public Transform bottomWall;
    public Transform topWall;

    private Vector3 currentFlyTarget;
    private float flySpeed = 8f;

    private Vector3 descendTarget;

    public GameObject explosionParticlePrefab;

    public Transform shootPoint;
    public GameObject projectilePrefab;

    private Rigidbody rb;
    private Animator anim;
    private GameObject dragonModel;

    private enum State { Idle, PreparingAttack, Shooting, FlyingRandom, Descending, None }
    private State currentState;

    //InfoEnemy
    public int max_health;
    public int health;

    public bool dragonDead;

    public GameObject hurtParticlePrefab;
    public GameObject deathParticles;

    private float totalInvincivility;
    private float currentInvencivilityPassed;
    private float damageAnimationTime;
    private float currentdamageAimationTime;

    //Sounds
    private List<AudioClip> idleClips;
    private List<AudioClip> hurtClips;
    private List<AudioClip> flyClips;
    private List<AudioClip> explosionClips;
    private AudioClip deathClip;
    private AudioClip shootClip;

    //idle sound controll
    public float max_time_idle;
    public float min_time_idle;
    public float idle_timer;

    //fly sound controll
    public float max_time_fly_sound;
    public float min_time_fly_sound;
    public float fly_timer_sound;

    //Spawn powerups
    public GameObject powerUpPrefab;
    public List<Transform> powerUpSpawners;
    List<PowerUpBehaviour.PowerUpTypes> chosenPowerUps = 
        new List<PowerUpBehaviour.PowerUpTypes>()
        {
            PowerUpBehaviour.PowerUpTypes.Totem,
            PowerUpBehaviour.PowerUpTypes.Small,
            PowerUpBehaviour.PowerUpTypes.Big,
            PowerUpBehaviour.PowerUpTypes.Magnet,
            PowerUpBehaviour.PowerUpTypes.Clone,
            PowerUpBehaviour.PowerUpTypes.Shoot,
            PowerUpBehaviour.PowerUpTypes.Barrier
        };

    private void Start()
    {
        rb = GetComponent<Rigidbody>();
        anim = GetComponentInChildren<Animator>();
        dragonModel = transform.GetChild(0).gameObject;

        GameManager.Instance.isFinalBoss = true;

        left = leftWall.position.x - 2f;
        right = rightWall.position.x + 2f;
        bottom = bottomWall.position.z + 2f;
        top = topWall.position.z - 2f;

        baseY = basePoint.position.y;

        max_health = 10;
        health = max_health;
        UIBehaviour.Instance.drawBossUI(health, max_health);

        dragonDead = false;

        totalInvincivility = 1.5f;
        currentInvencivilityPassed = 0;
        damageAnimationTime = 0.2f;
        currentdamageAimationTime = 0;

        idleClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Dragon/Idle").ToList();
        hurtClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Dragon/Hurt").ToList();
        flyClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Dragon/Fly").ToList();
        explosionClips = Resources.LoadAll<AudioClip>("Audio/Blocks/Explosion").ToList();
        idleClips = Resources.LoadAll<AudioClip>("Audio/Mobs/Dragon/Idle").ToList();
        deathClip = Resources.Load<AudioClip>("Audio/Mobs/Dragon/Ender_dragon_death");
        shootClip = Resources.Load<AudioClip>("Audio/Mobs/Dragon/Ender_dragon_shoot");

        max_time_idle = 10f;
        min_time_idle = 4f;
        idle_timer = Random.Range(min_time_idle, max_time_idle);

        max_time_fly_sound = 2f;
        min_time_fly_sound = 0.5f;
        fly_timer_sound = Random.Range(min_time_fly_sound, max_time_fly_sound);

        currentState = State.Idle;
        ResetShootTimer();
    }

    private void Update()
    {
        if (!dragonDead)
        {
            switch (currentState)
            {
                case State.Idle:
                    HandleIdle();
                    break;
                case State.PreparingAttack:
                    break;
                case State.Shooting:
                    break;
                case State.FlyingRandom:
                    HandleFlyingRandom();
                    break;
                case State.Descending:
                    HandleDescending();
                    break;
                case State.None:
                    break;
            }

            if (currentState == State.Idle || currentState == State.PreparingAttack || currentState == State.Shooting)
            {
                LookAtPlayer();
            }

            //Control invencibilitat
            if (currentInvencivilityPassed > 0)
            {
                currentInvencivilityPassed -= Time.deltaTime;
                currentdamageAimationTime -= Time.deltaTime;
                if (currentdamageAimationTime <= 0)
                {
                    currentdamageAimationTime = damageAnimationTime;
                    GameObject go = gameObject.transform.GetChild(0).transform.GetChild(0).gameObject;
                    go.SetActive(!go.activeSelf);
                }
                if (currentInvencivilityPassed <= 0) gameObject.transform.GetChild(0).transform.GetChild(0).gameObject.SetActive(true);
            }

            //Control idle sounds
            idle_timer -= Time.deltaTime;
            if (idle_timer <= 0)
            {
                int index = Random.Range(0, idleClips.Count);
                AudioSource.PlayClipAtPoint(idleClips[index], Camera.main.transform.position);
                idle_timer = Random.Range(min_time_idle, max_time_idle);
            }

            if (currentState == State.PreparingAttack || currentState == State.Shooting || currentState == State.FlyingRandom)
            {
                fly_timer_sound -= Time.deltaTime;
                if (fly_timer_sound <= 0)
                {
                    int index = Random.Range(0, flyClips.Count);
                    AudioSource.PlayClipAtPoint(flyClips[index], Camera.main.transform.position);
                    fly_timer_sound = Random.Range(min_time_fly_sound, max_time_fly_sound);
                }
            }
        }
    }

    void HandleIdle()
    {
        shootTimer -= Time.deltaTime;
        if (shootTimer <= 0)
        {
            StartCoroutine(PrepareAttack());
        }
    }

    IEnumerator PrepareAttack()
    {
        currentState = State.PreparingAttack;
        anim.SetTrigger("PrepareAttack"); // Debes configurar esto en el Animator

        yield return new WaitForSeconds(2f); // Tiempo de carga antes de disparar

        StartCoroutine(Shoot());
    }

    IEnumerator Shoot()
    {
        currentState = State.Shooting;

        int shots = 4;
        float interval = 0.5f;
        float longInterval = 1.5f;
        for (int i = 0; i < shots; i++)
        {
            ShootProjectileAtPlayer(i+1, shots);
            if (i >= shots - 2) yield return new WaitForSeconds(longInterval);
            else yield return new WaitForSeconds(interval);
        }

        ResetFlyTimer();
        anim.SetTrigger("Fly"); // Cambia animación al modo vuelo
        StartFlyingRandom();
    }

    void ShootProjectileAtPlayer(int current, int total)
    {
        GameObject player = GameManager.Instance.paddle.gameObject;
        if (player == null) return;

        Vector3 direction = (player.transform.position - shootPoint.position).normalized;

        AudioSource.PlayClipAtPoint(shootClip, Camera.main.transform.position);

        GameObject proj = Instantiate(projectilePrefab, shootPoint.position, Quaternion.LookRotation(direction));
        if (current == total) proj.GetComponent<DragonBullet>().isLast = true;
    }
    void StartFlyingRandom()
    {
        currentState = State.FlyingRandom;
        flyTimer = Random.Range(min_time_fly, max_time_fly);
        dragonModel.GetComponent<Animator>().SetTrigger("Fly");

        // Guardar altura actual
        ascending = true;

        // Establecer primera posición objetivo de vuelo horizontal
        currentFlyTarget = GetRandomPositionWithinBounds();
        currentFlyTarget.y = baseY; // se mantendrá hasta terminar de ascender

        createPowerUps();
    }

    private void createPowerUps()
    {
        //Aleatoritza l'ordre
        for (int i = 0; i < chosenPowerUps.Count; i++)
        {
            int randIndex = Random.Range(i, chosenPowerUps.Count);
            (chosenPowerUps[i], chosenPowerUps[randIndex]) = (chosenPowerUps[randIndex], chosenPowerUps[i]);
        }

        // Instancia els powerups
        for (int i = 0; i < powerUpSpawners.Count; ++i)
        {
            GameObject pu = Instantiate(powerUpPrefab, powerUpSpawners[i].position, Quaternion.identity);
            pu.GetComponent<PowerUpBehaviour>().DeactivateAllModels();
            pu.GetComponent<PowerUpBehaviour>().selectPU(chosenPowerUps[i]);
        }
    }

    void HandleFlyingRandom()
    {
        flyTimer -= Time.deltaTime;

        // Subir en Y hasta alcanzar targetFlyHeight
        if (ascending)
        {
            float newY = Mathf.MoveTowards(transform.position.y, targetFlyHeight, flyAscendSpeed * Time.deltaTime);
            transform.position = new Vector3(transform.position.x, newY, transform.position.z);

            if (Mathf.Approximately(transform.position.y, targetFlyHeight))
            {
                ascending = false;
                currentFlyTarget.y = targetFlyHeight;
            }

            return; // esperamos a terminar de subir antes de moverse horizontalmente
        }

        // Movimiento horizontal mientras vuela
        if (Vector3.Distance(transform.position, currentFlyTarget) < 0.2f)
        {
            currentFlyTarget = GetRandomPositionWithinBounds();
            currentFlyTarget.y = targetFlyHeight; // mantener altura fija
        }

        Vector3 direction = (currentFlyTarget - transform.position).normalized;
        direction.y = 0; // ignoramos componente vertical

        transform.position += direction * flySpeed * Time.deltaTime;

        if (direction != Vector3.zero)
        {
            Quaternion targetRotation = Quaternion.LookRotation(direction, Vector3.up);
            dragonModel.transform.rotation = Quaternion.Slerp(dragonModel.transform.rotation, targetRotation, 5f * Time.deltaTime);
        }

        if (flyTimer <= 0)
        {
            descendTarget = GetRandomGroundPosition();  // X, Z aleatorios, Y = baseY
            StartCoroutine(BeginDescending());
        }
    }

    IEnumerator BeginDescending()
    {
        currentState = State.None; // temporal para bloquear otros estados
        anim.SetTrigger("Descend");

        yield return new WaitForSeconds(0.5f); 

        currentState = State.Descending;
    }
    Vector3 GetRandomGroundPosition()
    {
        float x = Random.Range(left, right);
        float z = Random.Range(bottom, top);
        return new Vector3(x, baseY, z);
    }

    void HandleDescending()
    {
        float descendSpeed = 12f;

        transform.position = Vector3.MoveTowards(transform.position, descendTarget, descendSpeed * Time.deltaTime);

        Vector3 direction = (descendTarget - transform.position).normalized;
        if (direction != Vector3.zero)
        {
            Quaternion targetRotation = Quaternion.LookRotation(direction, Vector3.up);
            dragonModel.transform.rotation = Quaternion.Slerp(dragonModel.transform.rotation, targetRotation, 5f * Time.deltaTime);
        }

        if (Vector3.Distance(transform.position, descendTarget) < 0.1f)
        {
            int index = Random.Range(0, explosionClips.Count);
            AudioSource.PlayClipAtPoint(explosionClips[index], Camera.main.transform.position);
            GameObject particle;
            if (hurtParticlePrefab != null) particle = Instantiate(explosionParticlePrefab, basePoint.position, Quaternion.identity);

            createBulletRing();
            anim.SetTrigger("Idle");
            ResetShootTimer();
            currentState = State.Idle;
        }
    }

    void createBulletRing()
    {
        int bulletCount = 12;
        float angleStep = 360f / bulletCount;

        for (int i = 0; i < bulletCount; i++)
        {
            float angle = i * angleStep;
            Vector3 direction = new Vector3(Mathf.Sin(angle * Mathf.Deg2Rad), 0f, Mathf.Cos(angle * Mathf.Deg2Rad)).normalized;
            Quaternion rotation = Quaternion.LookRotation(direction, Vector3.up);

            Instantiate(projectilePrefab, fallShootPoint.position, rotation);
        }
    }


    void LookAtPlayer()
    {
        GameObject player = GameManager.Instance.paddle.gameObject;
        if (player == null) return;

        Vector3 direction = (player.transform.position - transform.position);
        direction.y = 0f; // Solo rotación en el plano XZ
        if (direction.magnitude > 0.1f)
        {
            Quaternion targetRotation = Quaternion.LookRotation(direction.normalized, Vector3.up);
            dragonModel.transform.rotation = Quaternion.Slerp(dragonModel.transform.rotation, targetRotation, 5f * Time.deltaTime);
        }
    }

    void ResetShootTimer()
    {
        shootTimer = Random.Range(min_time_shoot, max_time_shoot);
    }

    void ResetFlyTimer()
    {
        flyTimer = Random.Range(min_time_fly, max_time_fly);
    }

    Vector3 GetRandomPositionWithinBounds()
    {
        float x = Random.Range(left, right);
        float z = Random.Range(bottom, top);
        float y = transform.position.y; // mantener altura
        return new Vector3(x, y, z);
    }

    private void getHurt()
    {
        if (currentInvencivilityPassed <= 0)
        {
            --health;
            UIBehaviour.Instance.drawBossUI(health, max_health);
            //Die
            if (health <= 0)
            {
                dragonDead = true;
                anim.SetTrigger("Die");
                AudioSource.PlayClipAtPoint(deathClip, Camera.main.transform.position);
                GameObject particle;
                if (deathParticles != null) particle = Instantiate(deathParticles, transform.position, Quaternion.identity);
                GameManager.Instance.gameScore += 10000;
                Destroy(gameObject, 7f);
            }
            else
            {
                currentInvencivilityPassed = totalInvincivility;
                currentdamageAimationTime = damageAnimationTime;
                gameObject.transform.GetChild(0).transform.GetChild(0).gameObject.SetActive(false);

                int index = Random.Range(0, hurtClips.Count);
                AudioSource.PlayClipAtPoint(hurtClips[index], Camera.main.transform.position);
                GameObject particle;
                if (hurtParticlePrefab != null) particle = Instantiate(hurtParticlePrefab, transform.position, Quaternion.identity);
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Ball"))
        {
            getHurt();
        }
        else if (other.gameObject.tag == "Bullet")
        {
            other.gameObject.GetComponent<BulletBehaviour>().Break();
            getHurt();
        }
    }
}
