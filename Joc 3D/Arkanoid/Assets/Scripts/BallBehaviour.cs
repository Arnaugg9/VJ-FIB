using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class BallBehaviour : MonoBehaviour
{
    //components
    private Rigidbody _rb;
    private PaddleBehaviour paddle;
    private ParticleSystem _particleSystem;

    //Movement controll
    public int speed;
    private Vector3 _dir;
    private float _paddleOffset;
        //VELOCIDAD VARIABLE CON TIEMPO -> Dificultad

    //triggers
    private bool _first_collision;
    private bool _wasShoot;
    public bool ballDead;

    //Timers
    private float _powerTimer;
    private const float _PU_POWER_DURATION = 8;

    private void Awake()
    {
        _rb = GetComponent<Rigidbody>();
        paddle = GameManager.Instance.paddle;
        _particleSystem = GetComponent<ParticleSystem>();
        GameManager.Instance.activeBalls.Add(this);
    }

    // Start is called before the first frame update
    void Start()
    {
        if (!_wasShoot)
        {
            _rb.isKinematic = true;
            _dir = Vector3.zero;
            transform.position = paddle.transform.position + new Vector3(_paddleOffset, 0.0f, 0.75f);

            _first_collision = true;
            _wasShoot = false;
        }
        ballDead = false;
        _paddleOffset = 0.0f;
        _powerTimer = 0;
        speed = 8;
    }

    public void Restart()
    {
        _rb.isKinematic = true;
        _dir = Vector3.zero;
        _first_collision = true;
        _paddleOffset = 0;
        transform.position = paddle.transform.position + new Vector3(_paddleOffset, 0.0f, 0.75f);

        _wasShoot = false;
        ballDead = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (GameManager.Instance.levelStarted)
        {
            //Timer updates
            if (_powerTimer > 0)
            {
                _powerTimer -= Time.deltaTime;
                UIBehaviour.Instance.updateUI("power", (int)_powerTimer);
                if (_powerTimer <= 0)
                {
                    UIBehaviour.Instance.ch_stateItemUI("power", false);
                    setPower(false, 0);
                }
            }

            //Input control
            if (!ballDead && !_wasShoot && Input.GetKeyUp(KeyCode.Space))
            {
                _wasShoot = true;
                _rb.isKinematic = false;
                _dir = new Vector3(Random.Range(-0.75f, 0.75f), 0, 1);
                _rb.velocity = _dir * speed;
                paddle.playHitSound();
            }

            if (ballDead && Input.GetKeyUp(KeyCode.Space)) Restart();
        }
    }

    private void FixedUpdate()
    {
        if (!_wasShoot && !ballDead)
        {
            Vector3 newPos = transform.position;
            transform.position = paddle.transform.position + new Vector3(_paddleOffset, 0, 0.75f);
        }
        else if (!ballDead)
        {
            Vector3 velocity = _rb.velocity;

            //Normaliza la velocidad
            if (velocity.magnitude != speed)
            {
                velocity = velocity.normalized * speed;
            }

            //Assegurar una minima velocitat vertical
            if (Mathf.Abs(velocity.z) < 0.6f) 
            {
                float sign = Mathf.Sign(velocity.z) != 0 ? Mathf.Sign(velocity.z) : 1f;
                velocity.z = 0.6f * sign; 

                // Renormalizas para mantener la velocidad total
                velocity = velocity.normalized * speed;
            }

            _rb.velocity = velocity;
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Paddle")
        {
            if (paddle.magnetRemain > 0)
            {
                _wasShoot = false;
                _first_collision = false;
                _rb.isKinematic = true;
                _paddleOffset = transform.position.x - paddle.transform.position.x;
                --paddle.magnetRemain;
                if (paddle.magnetRemain > 0) UIBehaviour.Instance.updateUI("magnet", paddle.magnetRemain);
                else UIBehaviour.Instance.ch_stateItemUI("magnet", false);
            }
            else if (!_first_collision)
            {
                paddle.playHitSound();
                // Treu punt on ha chocat
                Vector3 hitPoint = collision.GetContact(0).point;
                Vector3 palaCenter = collision.transform.position;

                // Diferencia entre lloc on colpeja i centre pala
                float offset = hitPoint.x - palaCenter.x;
                float width = collision.collider.bounds.size.x;
                offset = offset / (width / 2f);

                // Nova direccio en funcio del offset
                Vector3 newDir = new Vector3(offset, 0, 1).normalized;

                // Aplica la nueva direcció
                _rb.velocity = newDir * _rb.velocity.magnitude;
            }
            else
            {
                paddle.playHitSound();
                _first_collision = false;
            }
        }
        _particleSystem.Play();
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "DeathZone")
        {
            ballDead = true;
            if (GameManager.Instance.activeBalls.Count > 1)
            {
                GameManager.Instance.activeBalls.Remove(this);
                Destroy(gameObject);
            }
            else
            {
                GameManager.Instance.loseLife();
            }
        }
    }

    public void setPower(bool value, float time)
    {
        if (value)
        {
            if (time == 0) _powerTimer += _PU_POWER_DURATION;
            else _powerTimer = time;
        }
        Physics.IgnoreLayerCollision(gameObject.layer, LayerMask.NameToLayer("Block"), value);
        transform.GetChild(1).gameObject.SetActive(value);
    }

    public float getPowerTime()
    {
        return _powerTimer;
    }

    public void initAfterClone(Vector3 dir)
    {
        _rb.isKinematic = false;
        _dir = dir;
        _rb.velocity = _dir * speed;

        _first_collision = false;
        _wasShoot = true;
    }
}
