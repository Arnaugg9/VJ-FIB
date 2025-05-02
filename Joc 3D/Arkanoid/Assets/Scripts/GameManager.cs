using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    //Scenes Managment
    public enum scenes {Level1, };

    //Sound Managment
    private AudioSource _audioSource;

    //Scripts Managment
    public PaddleBehaviour paddle;

    //Game State Managment
    

    private void Awake()
    {
        if (Instance != null)
        {
            Destroy(gameObject);
            return;
        }
        Instance = this;
        DontDestroyOnLoad(gameObject);

        _audioSource = GetComponent<AudioSource>();

    }

    // Start is called before the first frame update
    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
      
    }

    public void changePaddleSize(float size)
    {
        paddle.changeSize(size);
    }
}
