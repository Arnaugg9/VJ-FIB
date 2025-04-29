using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallBehaviour : MonoBehaviour
{
    //components
    private Rigidbody _rb;
    public GameObject paddle;

    //Movement controll
    public int speed;
    private Vector3 _dir;
    private bool _first_collision;
        //VELOCIDAD VARIABLE CON TIEMPO -> Dificultad

    //triggers
    private bool _wasShoot;
    private bool _ballDead;

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _rb.isKinematic = true; 
        _dir = Vector3.zero;
        _first_collision = true;
        transform.position = paddle.transform.position + new Vector3(0, 0.5f, 0.5f);

        _wasShoot = false;
        _ballDead = false;
    }

    public void Restart()
    {
        _rb.isKinematic = true;
        _dir = Vector3.zero;
        _first_collision = true;
        transform.position = paddle.transform.position + new Vector3(0, 0.5f, 0.5f);

        _wasShoot = false;
        _ballDead = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (!_ballDead && !_wasShoot && Input.GetKeyUp(KeyCode.Space))
        {
            _wasShoot = true;
            _rb.isKinematic = false;
            _dir = new Vector3(Random.Range(-0.75f, 0.75f), 0, 1);
            _rb.velocity = _dir * speed;
        }

        if (_ballDead && Input.GetKeyUp(KeyCode.Space)) Restart();
    }

    private void FixedUpdate()
    {
        if (!_wasShoot && !_ballDead)
        {
            transform.position = paddle.transform.position + new Vector3(0, 0, 0.75f);
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Paddle")
        {
            if (!_first_collision)
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
        }
    }
}
