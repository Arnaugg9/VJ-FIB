using Unity.VisualScripting;
using UnityEngine;

public class PowerUpBehaviour : MonoBehaviour
{
    public enum PowerUpTypes { Big, Small, Power, Magnet, Clone, Shoot, Barrier, Totem, Nextlvl };

    private Rigidbody _rb;

    public int fallSpeed;

    public bool canSpawnNextLvl;
    public bool isFinalBoss;

    // Aquí declararemos las referencias a los GameObjects de los modelos
    // Asignaremos estos en el editor de Unity
    public GameObject bigPowerUpModel;
    public GameObject smallPowerUpModel;
    public GameObject powerPowerUpModel;
    public GameObject magnetPowerUpModel;
    public GameObject clonePowerUpModel;
    public GameObject shootPowerUpModel;
    public GameObject barrierPowerUpModel;
    public GameObject totemPowerUpModel;
    public GameObject nextLvlPowerUpModel;


    public AudioClip itemPickupClip;


    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();

        isFinalBoss = GameManager.Instance.isFinalBoss;
        if (!isFinalBoss)
        {
            canSpawnNextLvl = GameManager.Instance.canSpawnNextLvl;
            int rand;
            if (canSpawnNextLvl)
            {
                rand = Random.Range(0, 13);
                if (rand >= 8) rand = 8;
            }
            else rand = Random.Range(0, 8);
            selectPU((PowerUpTypes)rand);
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (GameManager.Instance.levelStarted)
        {
            Vector3 newPos = _rb.position + Vector3.back * fallSpeed * Time.fixedDeltaTime;
            _rb.MovePosition(newPos);
        }
    }

    public void selectPU(PowerUpTypes n)
    {
        // Primero, desactiva todos los modelos para asegurarte de que solo se muestre el correcto
        DeactivateAllModels();

        if (n == PowerUpTypes.Big)
        {
            tag = "BigPU";
            if (bigPowerUpModel != null) bigPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Small)
        {
            tag = "SmallPU";
            if (smallPowerUpModel != null) smallPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Power)
        {
            tag = "PowerPU";
            if (powerPowerUpModel != null) powerPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Magnet)
        {
            tag = "MagnetPU";
            if (magnetPowerUpModel != null) magnetPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Clone)
        {
            tag = "ClonePU";
            if (clonePowerUpModel != null) clonePowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Shoot)
        {
            tag = "ShootPU";
            if (shootPowerUpModel != null) shootPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Barrier)
        {
            tag = "BarrierPU";
            if (barrierPowerUpModel != null) barrierPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Totem)
        {
            tag = "TotemPU";
            if (totemPowerUpModel != null) totemPowerUpModel.SetActive(true);
        }
        else if (n == PowerUpTypes.Nextlvl)
        {
            tag = "NextlvlPU";
            if (nextLvlPowerUpModel != null) nextLvlPowerUpModel.SetActive(true);
        }
            print("I am " + tag);
    }

    // Nuevo método para desactivar todos los modelos.
    public void DeactivateAllModels()
    {
        if (bigPowerUpModel != null) bigPowerUpModel.SetActive(false);
        if (smallPowerUpModel != null) smallPowerUpModel.SetActive(false);
        if (powerPowerUpModel != null) powerPowerUpModel.SetActive(false);
        if (magnetPowerUpModel != null) magnetPowerUpModel.SetActive(false);
        if (clonePowerUpModel != null) clonePowerUpModel.SetActive(false);
        if (shootPowerUpModel != null) shootPowerUpModel.SetActive(false);
        if (barrierPowerUpModel != null) barrierPowerUpModel.SetActive(false);
        if (totemPowerUpModel != null) totemPowerUpModel.SetActive(false);
        if (nextLvlPowerUpModel != null) nextLvlPowerUpModel.SetActive(false);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Paddle")
        {
            GameManager.Instance.gameScore += 500;

            if (tag == "SmallPU")
            {
                GameManager.Instance.changePaddleSize("small");
            }
            else if (tag == "BigPU")
            {
                GameManager.Instance.changePaddleSize("big");
            }
            else if (tag == "PowerPU")
            {
                GameManager.Instance.activatePower();
            }
            else if (tag == "MagnetPU")
            {
                GameManager.Instance.paddle.activateMagnet();
            }
            else if (tag == "ClonePU")
            {
                GameManager.Instance.cloneBall(GameManager.Instance.activeBalls[0].gameObject.transform.position);
            }
            else if (tag == "ShootPU")
            {
                GameManager.Instance.paddle.startShooting();
            }
            else if (tag == "BarrierPU")
            {
                GameManager.Instance.spawnBarrier();
            }
            else if (tag == "TotemPU")
            {
                GameManager.Instance.getTotem();
            }
            else if (tag == "NextlvlPU")
            {
                GameManager.Instance.gotoNextLvl();
            }
            
            AudioSource.PlayClipAtPoint(itemPickupClip, Camera.main.transform.position);
            Break();
        }
        if (collision.gameObject.tag == "DeathZone")
        {
            Break();
        }
    }

    public void Break()
    {
        Destroy(gameObject);
    }
}