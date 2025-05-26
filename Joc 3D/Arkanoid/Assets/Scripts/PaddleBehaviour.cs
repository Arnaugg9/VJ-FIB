using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;


public class PaddleBehaviour : MonoBehaviour
{
    //Components
    private Rigidbody _rb;
    private AudioSource _audioSource;
    private List<AudioClip> hitClips;
    public AudioClip shootClip;

    //Movement
    private Vector3 _dir;
    public float speed;
    public Transform leftWall;
    public Transform rightWall;
    public float wallWidth;

    //Properties
    public float paddleSize;

    //Timers
    private float _sizeTimer;
    private const float _PU_SIZE_DURATION = 8;
    public int magnetRemain;
    private int _nShoots;
    private const int _N_SHOOTS = 10;
    private float _nextShoot;
    private const float _TIME_BETWEEN_SHOOT = 2;

    //Prefabs
    public GameObject bulletPrefab;

    private void Awake()
    {
        GameManager.Instance.paddle = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        _rb = GetComponent<Rigidbody>();
        _audioSource = GetComponent<AudioSource>();
        _audioSource.volume = 0.4f;
        hitClips = Resources.LoadAll<AudioClip>("Audio/Sword").ToList();
        _dir = Vector3.zero;

        _sizeTimer = 0;
        magnetRemain = 0;
    }

    // Update is called once per frame
    void Update()
    {
        //Timer updates
        if (_sizeTimer > 0)
        {
            _sizeTimer -= Time.deltaTime;
            UIBehaviour.Instance.updateUI("big", (int)_sizeTimer);
            UIBehaviour.Instance.updateUI("small", (int)_sizeTimer);
            if (_sizeTimer <= 0)
            {
                paddleSize = 2.75f;
                updateSize();
                UIBehaviour.Instance.ch_stateItemUI("big", false);
                UIBehaviour.Instance.ch_stateItemUI("small", false);

            }
        }

        if (_nShoots > 0)
        {
            _nextShoot -= Time.deltaTime;
            if (_nextShoot <= 0)
            {
                --_nShoots;
                if (_nShoots > 0) UIBehaviour.Instance.updateUI("shoot", _nShoots);
                else UIBehaviour.Instance.ch_stateItemUI("shoot", false);
                    shootBullets();
                _nextShoot = _TIME_BETWEEN_SHOOT;
            }
        }

        //Input controll
        _dir = Vector3.zero;
        if (Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow))
        {
            _dir = Vector3.right;
        }
        if (Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow))
        {
            _dir = Vector3.left;
        }
    }

    private void FixedUpdate()
    {
        Vector3 newPos = _rb.position + _dir * speed * Time.fixedDeltaTime;
        if (newPos.x + paddleSize / 2 >= rightWall.position.x - wallWidth / 2) newPos.x = rightWall.position.x - wallWidth / 2 - paddleSize / 2;
        if (newPos.x - paddleSize / 2 <= leftWall.position.x + wallWidth / 2) newPos.x = leftWall.position.x + wallWidth / 2 + paddleSize / 2;
        _rb.MovePosition(newPos);
    }

    public void changeSize(string size)
    {
        if (paddleSize > 3) UIBehaviour.Instance.ch_stateItemUI("big", false);
        else if (paddleSize < 2) UIBehaviour.Instance.ch_stateItemUI("small", false);
        UIBehaviour.Instance.ch_stateItemUI(size, true);
        _sizeTimer = _PU_SIZE_DURATION;
        paddleSize = (size == "big" ? 3.75f : 1.75f);
        updateSize();
    }

    private void updateSize()
    {
        Vector3 newSize = GetComponent<BoxCollider>().size;
        newSize.x = paddleSize;
        GetComponent<BoxCollider>().size = newSize;
        newSize.y = transform.GetChild(0).localScale.y;
        transform.GetChild(0).localScale = newSize;
    }

    public void activateMagnet()
    {
        UIBehaviour.Instance.ch_stateItemUI("magnet", true);
        magnetRemain += 5;
        UIBehaviour.Instance.updateUI("magnet", magnetRemain);
    }

    public void shootBullets()
    {
        Vector3 posPaddle = transform.position;
        Vector3 posB = posPaddle;
        posB.x -= paddleSize / 2 + 0.5f;
        GameObject bullet1 = Instantiate(bulletPrefab, posB, Quaternion.identity);
        posB.x += paddleSize - 1f;
        GameObject bullet2 = Instantiate(bulletPrefab, posB, Quaternion.identity);
        _audioSource.PlayOneShot(shootClip);
    }

    public void startShooting()
    {
        UIBehaviour.Instance.ch_stateItemUI("shoot", true);
        _nShoots = _N_SHOOTS;
        _nextShoot = 0;
        UIBehaviour.Instance.updateUI("shoot", _nShoots);
    }

    public void playHitSound()
    {
        int index = UnityEngine.Random.Range(0, hitClips.Count);
        _audioSource.PlayOneShot(hitClips[index]);
        //AudioSource.PlayClipAtPoint(hitClips[index], Camera.main.transform.position);
    }
}
