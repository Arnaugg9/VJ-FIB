using Unity.VisualScripting;
using UnityEngine;

public class PowerUpBehaviour : MonoBehaviour
{
    public enum PowerUpTypes { Big, Small, Power, Magnet, Clone, Shoot };

    private Rigidbody _rb;

    public int fallSpeed;

    // Aquí declararemos las referencias a los GameObjects de los modelos
    // Asignaremos estos en el editor de Unity
    public GameObject bigPowerUpModel;
    public GameObject smallPowerUpModel;
    public GameObject powerPowerUpModel;
    public GameObject magnetPowerUpModel;
    public GameObject clonePowerUpModel;
    public GameObject shootPowerUpModel;

    public AudioClip itemPickupClip;


    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();

        int rand = Random.Range(0, 6);
        selectPU((PowerUpTypes)rand);
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

    void selectPU(PowerUpTypes n)
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
        print("I am " + tag);
    }

    // Nuevo método para desactivar todos los modelos.
    void DeactivateAllModels()
    {
        if (bigPowerUpModel != null) bigPowerUpModel.SetActive(false);
        if (smallPowerUpModel != null) smallPowerUpModel.SetActive(false);
        if (powerPowerUpModel != null) powerPowerUpModel.SetActive(false);
        if (magnetPowerUpModel != null) magnetPowerUpModel.SetActive(false);
        if (clonePowerUpModel != null) clonePowerUpModel.SetActive(false);
        if (shootPowerUpModel != null) shootPowerUpModel.SetActive(false);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Paddle")
        {
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