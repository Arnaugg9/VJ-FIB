using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;
using static UnityEditorInternal.VersionControl.ListControl;

public class BallBehaviour : MonoBehaviour
{
    //components
    private Rigidbody _rb;
    private PaddleBehaviour paddle;

    //Movement controll
    public int speed;
    private Vector3 _dir;
    private float _paddleOffset;
        //VELOCIDAD VARIABLE CON TIEMPO -> Dificultad

    //triggers
    private bool _first_collision;
    private bool _wasShoot;
    private bool _ballDead;

    //Timers
    private float _powerTimer;
    private const float _PU_POWER_DURATION = 8;

    private void Awake()
    {
        GameManager.Instance.activeBalls.Add(this);
        _rb = GetComponent<Rigidbody>();
        paddle = GameManager.Instance.paddle;
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
        _ballDead = false;
        _paddleOffset = 0.0f;
        _powerTimer = 0;
    }

    public void Restart()
    {
        _rb.isKinematic = true;
        _dir = Vector3.zero;
        _first_collision = true;
        _paddleOffset = 0;
        transform.position = paddle.transform.position + new Vector3(_paddleOffset, 0.0f, 0.75f);

        _wasShoot = false;
        _ballDead = false;
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
                if (_powerTimer <= 0) setPower(false);
            }

            //Input control
            if (!_ballDead && !_wasShoot && Input.GetKeyUp(KeyCode.Space))
            {
                _wasShoot = true;
                _rb.isKinematic = false;
                _dir = new Vector3(Random.Range(-0.75f, 0.75f), 0, 1);
                _rb.velocity = _dir * speed;
            }

            if (_ballDead && Input.GetKeyUp(KeyCode.Space)) Restart();
        }
    }

    private void FixedUpdate()
    {
        if (GameManager.Instance.levelStarted)
        {
            if (!_wasShoot && !_ballDead)
            {
                Vector3 newPos = transform.position;
                transform.position = paddle.transform.position + new Vector3(_paddleOffset, 0, 0.75f);
            }
            else if (!_ballDead)
            {
                Vector3 velocity = _rb.velocity;

                if (Mathf.Abs(velocity.z) < 2.0f)
                {
                    float sign = Mathf.Sign(velocity.z) != 0 ? Mathf.Sign(velocity.z) : 1f;     //Per si de cas fos 0 va predeterminat endavant
                    velocity.z = 2.0f * sign;
                    velocity = velocity.normalized * speed;
                    _rb.velocity = velocity;
                }
            }
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
            }
            else if (!_first_collision)
            {
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
            else _first_collision = false;
        }
    }

    private void OnTriggerEnter(Collider collision)
    {
        if (collision.gameObject.tag == "DeathZone")
        {
            _ballDead = true;
            if(GameManager.Instance.activeBalls.Count > 1)
            {
                GameManager.Instance.activeBalls.Remove(this);
                Destroy(gameObject);
            }
        }
    }

    public void setPower(bool value)
    {
        if (value) _powerTimer = _PU_POWER_DURATION;
        Physics.IgnoreLayerCollision(gameObject.layer, LayerMask.NameToLayer("Block"), value);
        transform.GetChild(1).gameObject.SetActive(value);
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
