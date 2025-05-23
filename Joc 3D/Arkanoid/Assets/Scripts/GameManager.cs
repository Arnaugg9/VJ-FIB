using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    //Scenes Managment
    public enum scenes {MainMenu, Level1, Level2, Level3, Level4, Level5, Credits};
    public string activeScene;

    //Sound Managment
    private AudioSource _audioSource;

    //Prefabs
    public GameObject ballPrefab;

    //Scripts Managment
    public PaddleBehaviour paddle;
    public List<BallBehaviour> activeBalls;

    //Game State Managment
    public int gameScore;
    public int lives;
    public bool levelStarted;
    private bool _canNextLvl;

    public GodModeWalls godModeWall;
    public int blocksCurrent;
    public int blocksDestroyed;

    //UI Managment
    public TextMeshProUGUI scoreTxt;
    public TextMeshProUGUI livesTxt;
    public TextMeshProUGUI levelTxt;
    public GameObject NextLvlButton;

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
        levelStarted = false;
        gameScore = 0;
        lives = 3;
        blocksCurrent = 0;
    }

    // Update is called once per frame
    void Update()
    {
        //Text updates
        scoreTxt.text = "Score: " + gameScore;
        livesTxt.text = "Lives: " + lives;
        levelTxt.text = "Level " + SceneManager.GetActiveScene().buildIndex;

        if (levelStarted)
        {
            activeScene = SceneManager.GetActiveScene().name;

            //Scene change with number
            if (Input.GetKeyDown(KeyCode.G)) godModeWall.gameObject.SetActive(!godModeWall.gameObject.activeSelf);

            if (activeScene != "Level1" && (Input.GetKeyDown(KeyCode.Keypad1) || Input.GetKeyDown(KeyCode.Alpha1)))
                changeScene("Level1");
            if (activeScene != "Level2" && (Input.GetKeyDown(KeyCode.Keypad2) || Input.GetKeyDown(KeyCode.Alpha2)))
                changeScene("Level2");
            if (activeScene != "Level3" && (Input.GetKeyDown(KeyCode.Keypad3) || Input.GetKeyDown(KeyCode.Alpha3)))
                changeScene("Level3");
            if (activeScene != "Level4" && (Input.GetKeyDown(KeyCode.Keypad4) || Input.GetKeyDown(KeyCode.Alpha4)))
                changeScene("Level4");
            if (activeScene != "Level5" && (Input.GetKeyDown(KeyCode.Keypad5) || Input.GetKeyDown(KeyCode.Alpha5)))
                changeScene("Level5");

            _canNextLvl = checkNextLvl();
            if (_canNextLvl)
            {
                if (!NextLvlButton.activeSelf) NextLvlButton.SetActive(true);
                if (Input.GetKeyDown(KeyCode.N)) gotoNextLvl();
            }
        }
    }

    private bool checkNextLvl()
    {
        return (((float)blocksDestroyed / blocksCurrent) * 100 >= 95);
    }

    private void changeScene(string scene)
    {
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        NextLvlButton.SetActive(false);
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

    public void loseGame()
    {
        print("LOST");
    }

    public void activatePower()
    {
        for (int i = 0; i < activeBalls.Count; ++i)
        {
            activeBalls[i].setPower(true);
        }
    }

    public void gotoNextLvl()
    {
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        NextLvlButton.SetActive(false);
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }
}
