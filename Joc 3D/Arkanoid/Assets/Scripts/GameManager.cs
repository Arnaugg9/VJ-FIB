using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    //Scenes Managment
    public enum scenes {MainMenu, Level1, Level2, Level3, Level4, Level5, Credits};

    //Sound Managment
    private AudioSource _audioSource;

    //Scripts Managment
    public PaddleBehaviour paddle;

    //Game State Managment
    public GodModeWalls godModeWall;
    

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
        if (Input.GetKeyDown(KeyCode.G)) godModeWall.gameObject.SetActive(!godModeWall.gameObject.activeSelf);

        if (SceneManager.GetActiveScene().name != "Level1" && (Input.GetKeyDown(KeyCode.Keypad1) || Input.GetKeyDown(KeyCode.Alpha1))) 
            SceneManager.LoadScene("Level1");
        if (SceneManager.GetActiveScene().name != "Level2" && (Input.GetKeyDown(KeyCode.Keypad2) || Input.GetKeyDown(KeyCode.Alpha2)))
            SceneManager.LoadScene("Level2");
        if (SceneManager.GetActiveScene().name != "Level3" && (Input.GetKeyDown(KeyCode.Keypad3) || Input.GetKeyDown(KeyCode.Alpha3)))
            SceneManager.LoadScene("Level3");
        if (SceneManager.GetActiveScene().name != "Level4" && (Input.GetKeyDown(KeyCode.Keypad4) || Input.GetKeyDown(KeyCode.Alpha4)))
            SceneManager.LoadScene("Level4");
        if (SceneManager.GetActiveScene().name != "Level5" && (Input.GetKeyDown(KeyCode.Keypad5) || Input.GetKeyDown(KeyCode.Alpha5)))
            SceneManager.LoadScene("Level5");

    }

    public void changePaddleSize(float size)
    {
        paddle.changeSize(size);
    }
}
