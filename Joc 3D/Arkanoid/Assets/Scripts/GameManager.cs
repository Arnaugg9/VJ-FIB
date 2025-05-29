using System;
using System.Collections.Generic;
using System.Linq;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using static UnityEngine.ParticleSystem;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    //Scenes Managment
    private Dictionary<int, String> scenes;

    //Sound Managment
    private AudioSource _audioSource;
    List<AudioClip> scene_themes;

    //Prefabs
    public GameObject ballPrefab;
    public GameObject barrierPrefab;
    public GameObject currentBarrier;

    //Scripts Managment
    public PaddleBehaviour paddle;
    public List<BallBehaviour> activeBalls;

    //Game State Managment
    public int gameScore;
    public int lives;
    public bool levelStarted;
    private bool _canNextLvl;

    public int nTotemsActive;
    public AudioClip totemClip;
    public GameObject totemParticles;

    public GodModeWalls godModeWall;
    public int blocksCurrent;
    public int blocksDestroyed;

    public int itemSpawnProbability;

    public WitherSpawn witherSpawner;

    //UI Managment
    public TextMeshProUGUI scoreTxt;
    public List<GameObject> lifeSlots;
    public Texture fullHeart;
    public Texture emptyHeart;
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
        scenes = new Dictionary<int, string>
        {
            { 0, "MainMenu" },
            { 1, "Level1" },
            { 2, "Level2" },
            { 3, "Level3" },
            { 4, "Level4" },
            { 5, "Level5" },
            { 6, "Credits" },
            { 7, "Null" }
        };

        levelStarted = false;
        gameScore = 0;
        lives = 3;
        nTotemsActive = 0;
        blocksCurrent = 0;
        itemSpawnProbability = getProbabilityByLevel(SceneManager.GetActiveScene().buildIndex);
        changeMusic(SceneManager.GetActiveScene().name);
        drawLife();
        currentBarrier = new GameObject();
    }

    public int getProbabilityByLevel(int s)
    {
        string scene = scenes[s];
        if (scene == "Level1") return 7;
        if (scene == "Level2") return 8;
        if (scene == "Level3") return 10;
        if (scene == "Level4") return 13;
        if (scene == "Level5") return 15;
        else return 0;
    }

    public void drawLife()
    {
        foreach(GameObject slot in lifeSlots)
        {
            slot.GetComponent<UnityEngine.UI.RawImage>().texture = emptyHeart; 
        }

        if (lives >= 1) lifeSlots[0].GetComponent<UnityEngine.UI.RawImage>().texture = fullHeart;
        if (lives >= 2) lifeSlots[1].GetComponent<UnityEngine.UI.RawImage>().texture = fullHeart;
        if (lives >= 3) lifeSlots[2].GetComponent<UnityEngine.UI.RawImage>().texture = fullHeart;

    }

    // Update is called once per frame
    void Update()
    {
        //Text updates
        scoreTxt.text = "Score: " + gameScore;
        levelTxt.text = "Level " + SceneManager.GetActiveScene().buildIndex;

        if (levelStarted)
        {
            //Scene change with number
            if (Input.GetKeyDown(KeyCode.G)) godModeWall.gameObject.SetActive(!godModeWall.gameObject.activeSelf);

            int activeScene = SceneManager.GetActiveScene().buildIndex;
            if (activeScene != 1 && (Input.GetKeyDown(KeyCode.Keypad1) || Input.GetKeyDown(KeyCode.Alpha1)))
                changeScene(1);
            if (activeScene != 2 && (Input.GetKeyDown(KeyCode.Keypad2) || Input.GetKeyDown(KeyCode.Alpha2)))
                changeScene(2);
            if (activeScene != 3 && (Input.GetKeyDown(KeyCode.Keypad3) || Input.GetKeyDown(KeyCode.Alpha3)))
                changeScene(3);
            if (activeScene != 4 && (Input.GetKeyDown(KeyCode.Keypad4) || Input.GetKeyDown(KeyCode.Alpha4)))
                changeScene(4);
            if (activeScene != 5 && (Input.GetKeyDown(KeyCode.Keypad5) || Input.GetKeyDown(KeyCode.Alpha5)))
                changeScene(5);

            if (activeScene == 4 && !witherSpawner.hasSpawned && brokenPercentage() >= 1)
            {
                witherSpawner.spawnWither();
            } 

            _canNextLvl = checkNextLvl();
            if (activeScene != 4 && _canNextLvl)
            {
                if (!NextLvlButton.activeSelf) NextLvlButton.SetActive(true);
                if (Input.GetKeyDown(KeyCode.N)) gotoNextLvl();
            }
            else if (activeScene == 4 && witherSpawner.isDead)
            {
                if (!NextLvlButton.activeSelf) NextLvlButton.SetActive(true);
                if (Input.GetKeyDown(KeyCode.N)) gotoNextLvl();
            }
        }

        if (!_audioSource.isPlaying && scene_themes.Count > 0)
        {
            playRandomClip();
        }

        if (activeBalls.Count > 1 || (activeBalls.Count > 0 && !activeBalls[0].ballDead)) UIBehaviour.Instance.updateUI("ball", activeBalls.Count);
        else UIBehaviour.Instance.updateUI("ball", 0);
    }

    private float brokenPercentage()
    {
        return (((float)blocksDestroyed / blocksCurrent) * 100);
    }
    private bool checkNextLvl()
    {
        return (((float)blocksDestroyed / blocksCurrent) * 100 >= 95);
    }

    private void changeScene(int scene)
    {
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        changeMusic(scenes[scene]);
        NextLvlButton.SetActive(false);
        itemSpawnProbability = getProbabilityByLevel(scene);
        UIBehaviour.Instance.drawBossUI(0, 0);
        nTotemsActive = 0;
        UIBehaviour.Instance.initInventory();
        SceneManager.LoadScene(scene);
    }

    private void changeMusic(String scene)
    {
        scene_themes = Resources.LoadAll<AudioClip>("Audio/OST/Levels").ToList();
        playRandomClip();
    }

    private void playRandomClip()
    {
        int index = UnityEngine.Random.Range(0, scene_themes.Count);
        AudioClip selectedClip = scene_themes[index];

        _audioSource.clip = selectedClip;
        _audioSource.loop = false;
        _audioSource.playOnAwake = false;
        _audioSource.Play();
    }

    public void spawnBarrier()
    {
        if (currentBarrier != null) Destroy(currentBarrier);
        currentBarrier = Instantiate(barrierPrefab, Vector3.zero, Quaternion.identity);
    }

    public void changePaddleSize(string size)
    {
        paddle.changeSize(size);
    }

    public void cloneBall(UnityEngine.Vector3 originalPos)
    {
        instantiateBall(originalPos, new Vector3(-1, 0, 1));
        instantiateBall(originalPos, new Vector3(1, 0, 1));
    }

    public void getTotem()
    {
        ++nTotemsActive;
        UIBehaviour.Instance.ch_stateItemUI("totem", true);
        UIBehaviour.Instance.updateUI("totem", nTotemsActive);
    }

    public void instantiateBall(Vector3 pos, Vector3 dir)
    {
        GameObject newBall = Instantiate(ballPrefab, pos, Quaternion.identity);
        newBall.GetComponent<BallBehaviour>().initAfterClone(dir);
        if (activeBalls[0].getPowerTime() > 0) newBall.GetComponent<BallBehaviour>().setPower(true, activeBalls[0].getPowerTime());
    }

    public void loseGame()
    {
        print("LOST");
    }

    public void activatePower()
    {
        UIBehaviour.Instance.ch_stateItemUI("power", true);
        for (int i = 0; i < activeBalls.Count; ++i)
        {
            activeBalls[i].setPower(true, 0);
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
        changeMusic(scenes[SceneManager.GetActiveScene().buildIndex + 1]);
        itemSpawnProbability = getProbabilityByLevel(SceneManager.GetActiveScene().buildIndex + 1);
        UIBehaviour.Instance.drawBossUI(0, 0);
        nTotemsActive = 0;
        UIBehaviour.Instance.initInventory();
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
    }

    public void loseLife()
    {

        if (nTotemsActive <= 0)
        {
            --lives;
            drawLife();
            if (lives < 0)
            {
                loseGame();
            }
        }
        else
        {
            AudioSource.PlayClipAtPoint(totemClip, Camera.main.transform.position);
            --nTotemsActive;
            GameObject particle = Instantiate(totemParticles, paddle.transform.position, Quaternion.identity);
            UIBehaviour.Instance.updateUI("totem", nTotemsActive);
            if (nTotemsActive <= 0)
            {
                UIBehaviour.Instance.ch_stateItemUI("totem", false);
            }
        }
    }
}
