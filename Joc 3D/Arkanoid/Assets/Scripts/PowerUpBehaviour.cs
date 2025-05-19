using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class PowerUpBehaviour : MonoBehaviour
{
    public enum PowerUpTypes {Big, Small, Power, Magnet, Clone, Shoot};

    private Rigidbody _rb;

    public int fallSpeed;


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
        if (n == PowerUpTypes.Big)
        {
            tag = "BigPU";
            GetComponentInChildren<Renderer>().material.color = Color.red;
        }
        else if (n == PowerUpTypes.Small)
        {
            tag = "SmallPU";
            GetComponentInChildren<Renderer>().material.color = Color.blue;
        }
        else if (n == PowerUpTypes.Power)
        {
            tag = "PowerPU";
            GetComponentInChildren<Renderer>().material.color = Color.magenta;
        }
        else if (n == PowerUpTypes.Magnet)
        {
            tag = "MagnetPU";
            GetComponentInChildren<Renderer>().material.color = Color.yellow;
        }
        else if (n == PowerUpTypes.Clone)
        {
            tag = "ClonePU";
            GetComponentInChildren<Renderer>().material.color = Color.green;
        }
        else if (n == PowerUpTypes.Shoot)
        {
            tag = "ShootPU";
            GetComponentInChildren<Renderer>().material.color = Color.grey;
        }
        print("I am " + tag);
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "Paddle")
        {
            if (tag == "SmallPU")
            {
                GameManager.Instance.changePaddleSize(1.75f);
            }
            else if (tag == "BigPU")
            {
                GameManager.Instance.changePaddleSize(3.75f);
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
            Break();
        }
    }
    public void Break()
    {
        Destroy(gameObject);
    }
}
