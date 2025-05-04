using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    //Scenes Managment
    public enum scenes {MainMenu, Level1, Level2, Level3, Level4, Level5, Credits};

    //Sound Managment
    private AudioSource _audioSource;

    //Prefabs
    public GameObject ballPrefab;

    //Scripts Managment
    public PaddleBehaviour paddle;
    public List<BallBehaviour> activeBalls;

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
        activeBalls = new List<BallBehaviour>();
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
            changeScene("Level1");
        if (SceneManager.GetActiveScene().name != "Level2" && (Input.GetKeyDown(KeyCode.Keypad2) || Input.GetKeyDown(KeyCode.Alpha2)))
            changeScene("Level2");
        if (SceneManager.GetActiveScene().name != "Level3" && (Input.GetKeyDown(KeyCode.Keypad3) || Input.GetKeyDown(KeyCode.Alpha3)))
            changeScene("Level3");
        if (SceneManager.GetActiveScene().name != "Level4" && (Input.GetKeyDown(KeyCode.Keypad4) || Input.GetKeyDown(KeyCode.Alpha4)))
            changeScene("Level4");
        if (SceneManager.GetActiveScene().name != "Level5" && (Input.GetKeyDown(KeyCode.Keypad5) || Input.GetKeyDown(KeyCode.Alpha5)))
            changeScene("Level5");

    }

    private void changeScene(string scene)
    {
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        SceneManager.LoadScene(scene);
    }

    public void changePaddleSize(float size)
    {
        paddle.changeSize(size);
    }

    public void cloneBall(UnityEngine.Vector3 originalPos)
    {
        instantiateBall(originalPos, new Vector3(-1, 0, 1));
        instantiateBall(originalPos, new Vector3(1, 0, 1));
    }

    public void instantiateBall(Vector3 pos, Vector3 dir)
    {
        GameObject newBall = Instantiate(ballPrefab, pos, Quaternion.identity);
        newBall.GetComponent<BallBehaviour>().initAfterClone(dir);
    }
}
