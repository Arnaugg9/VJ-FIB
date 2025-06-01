using Newtonsoft.Json.Linq;
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
    private string currentPlayerName;
    public int gameScore;
    public int lives;
    private int max_lives;
    public bool levelStarted;
    public bool canSpawnNextLvl;
    public AudioClip nextLvlClip;
    public List<AudioClip> playerHurtClips;

    public int nTotemsActive;
    public AudioClip totemClip;
    public GameObject totemParticles;

    public GodModeWalls godModeWall;
    public bool fullGodMode;
    public bool semiGodMode;
    public int blocksCurrent;
    public int blocksDestroyed;

    public int itemSpawnProbability;

    public WitherSpawn witherSpawner;

    public bool isFinalBoss;
    public DragonSpawner dragonSpawner;

    //UI Managment
    public GameObject Game_UI;
    public TextMeshProUGUI scoreTxt;
    public List<GameObject> lifeSlots;
    public Texture fullHeart;
    public Texture emptyHeart;
    public TextMeshProUGUI levelTxt;
    public GameObject NextLvlButton;

    //PAuse managment
    public PauseManager pauseManager;
    private AudioClip buttonClip;
    public bool paused = false;

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

        currentPlayerName = PlayerPrefs.GetString("LastPlayerName", "Anonymous");
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
        max_lives = 3;
        lives = max_lives;
        buttonClip = Resources.Load<AudioClip>("Audio/UI/Click_stereo");
        playerHurtClips = Resources.LoadAll<AudioClip>("Audio/Miscelaneous/PlayerHurt").ToList();
        nTotemsActive = 0;
        blocksCurrent = 0;
        fullGodMode = false;
        semiGodMode = false;
        itemSpawnProbability = getProbabilityByLevel(SceneManager.GetActiveScene().buildIndex);
        isFinalBoss = false;
        scene_themes = new List<AudioClip>();
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
        int activeScene = SceneManager.GetActiveScene().buildIndex;

        if (activeScene == 0 || activeScene == 6)
        {
            Game_UI.SetActive(false);
        }
        else if (!Game_UI.activeSelf) Game_UI.SetActive(true);

        if (Input.GetKeyDown(KeyCode.Escape) && activeScene != 0 && activeScene != 6)
        {
            _audioSource.PlayOneShot(buttonClip);
            if (!paused) pauseGame();
            else resume();
        }

        //Text updates
        scoreTxt.text = "Score: " + gameScore;
        levelTxt.text = "Level " + SceneManager.GetActiveScene().buildIndex;

        if (levelStarted && !paused)
        {
            //Scene change with number
            if (Input.GetKeyDown(KeyCode.G))
            {
                fullGodMode = !fullGodMode;
                semiGodMode = false;
            }
            if (Input.GetKeyDown(KeyCode.H))
            {
                semiGodMode = !semiGodMode;
                fullGodMode = false;
            }
            godModeWall.gameObject.SetActive(fullGodMode || semiGodMode);

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

            checkProgress(activeScene);
        }

        if (!_audioSource.isPlaying && scene_themes.Count > 0)
        {
            changeMusic(scenes[activeScene]);
            playRandomClip();
        }

        if (UIBehaviour.Instance != null)
        {
            if (activeBalls.Count > 1 || (activeBalls.Count > 0 && !activeBalls[0].ballDead)) UIBehaviour.Instance.updateUI("ball", activeBalls.Count);
            else UIBehaviour.Instance.updateUI("ball", 0);
        }
    }

    private void checkProgress(int activeScene)
    {
        float broken = brokenPercentage();
        //If level is 1 to 3 can spawnNextLvl Item when 95% cleared
        if (activeScene == 1 || activeScene == 2 || activeScene == 3)
        {
            canSpawnNextLvl = broken >= 95;
            if (broken >= 98)
            {
                if (!NextLvlButton.activeSelf) NextLvlButton.SetActive(true);
                if (Input.GetKeyDown(KeyCode.N)) gotoNextLvl();
            }

            if (broken >= 100) gotoNextLvl();
        }
        //If level 4 -> Spawn wither when broken is 50% and only can change level when 95% cleared and killed wither
        else if (activeScene == 4)
        {
            if (!witherSpawner.hasSpawned && broken >= 1)
            {
                witherSpawner.spawnWither();
            }

            canSpawnNextLvl = witherSpawner.isDead && (broken >= 95);

            if (witherSpawner.isDead && broken >= 98)
            {
                if (!NextLvlButton.activeSelf)
                {
                    UIBehaviour.Instance.drawBossUI(0, 0);
                    NextLvlButton.SetActive(true);
                }
                if (Input.GetKeyDown(KeyCode.N)) gotoNextLvl();
            }

            if (witherSpawner.isDead && (broken >= 100)) gotoNextLvl();
        }
        //If levle5 -> *Still have to do dragon fight
        else if (activeScene == 5)
        {
            if (!dragonSpawner.started && broken >= 1)
            {
                isFinalBoss = true;
                dragonSpawner.startFight();
            }

            if (dragonSpawner.isDead) {
                gotoNextLvl();
            }
        }
    }

    private float brokenPercentage()
    {
        return (((float)blocksDestroyed / blocksCurrent) * 100);
    }

    public void changeScene(int scene)
    {
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        changeMusic(scenes[scene]);
        NextLvlButton.SetActive(false);
        Physics.IgnoreLayerCollision(LayerMask.NameToLayer("Ball"), LayerMask.NameToLayer("Block"), false);
        itemSpawnProbability = getProbabilityByLevel(scene);
        UIBehaviour.Instance.drawBossUI(0, 0);
        nTotemsActive = 0;
        UIBehaviour.Instance.initInventory();
        isFinalBoss = false;
        SceneManager.LoadScene(scene);
    }

    private void changeMusic(String scene)
    {
        scene_themes.Clear();
        if (scene != "Level5") scene_themes = Resources.LoadAll<AudioClip>("Audio/OST/Levels/Chill").ToList();
        if (scene == "Level4" || scene == "Level5" || scene == "Level3") scene_themes = Resources.LoadAll<AudioClip>("Audio/OST/Levels/NoChill").ToList();
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
        bool powerActive = activeBalls[0].getPowerTime() > 0;
        GameObject newBall = Instantiate(ballPrefab, pos, Quaternion.identity);
        newBall.GetComponent<BallBehaviour>().initAfterClone(dir);
        newBall.GetComponent<BallBehaviour>().setPower(powerActive, activeBalls[0].getPowerTime());
    }

    public void EndGame(bool wonGame)
    {
        Debug.Log($"Game Over! Player: {currentPlayerName}, Final Score: {gameScore}. {(wonGame ? "You won!" : "You lost.")}");

        // Envía la puntuación al HighScoreManager
        if (HighScoreManager.Instance != null)
        {
            HighScoreManager.Instance.AddNewHighScore(currentPlayerName, gameScore);
        }
        else
        {
            Debug.LogError("HighScoreManager not found in scene!");
        }


        if (wonGame)
        {
            restartGameState();
            changeMusic("Credits");
            changeScene(6);
        }

        else
        {
            pauseDeath();
        }
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
        int nextSceneIndex = SceneManager.GetActiveScene().buildIndex + 1;

        // Si el siguiente nivel es "Credits"
        if (scenes.ContainsKey(nextSceneIndex) && scenes[nextSceneIndex] == "Credits")
        {
            SceneManager.LoadScene("Credits");
            return; 
        }

        activeBalls.Clear();
        paddle = null; 
        godModeWall = null; 
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        NextLvlButton.SetActive(false);
        Physics.IgnoreLayerCollision(LayerMask.NameToLayer("Ball"), LayerMask.NameToLayer("Block"), false);
        changeMusic(scenes[nextSceneIndex]); // Asegúrate de que scenes[nextSceneIndex] sea válido
        itemSpawnProbability = getProbabilityByLevel(nextSceneIndex);
        UIBehaviour.Instance.drawBossUI(0, 0);
        nTotemsActive = 0;
        UIBehaviour.Instance.initInventory();
        _audioSource.PlayOneShot(nextLvlClip);
        isFinalBoss = false;
        SceneManager.LoadScene(nextSceneIndex);
    }

    public void loseLife()
    {

        if (nTotemsActive <= 0)
        {
            if (!fullGodMode)
            {
                int rand = UnityEngine.Random.Range(0, playerHurtClips.Count);
                AudioSource.PlayClipAtPoint(playerHurtClips[rand], Camera.main.transform.position);
                --lives;
                drawLife();
                if (lives <= 0)
                {
                    EndGame(false);
                }
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

    //PAUSE FUNCTIONS
    private void pauseGame()
    {
        paused = true;
        Time.timeScale = 0f;
        pauseManager.ShowPauseMenu();
    } 

    private void pauseDeath()
    {
        pauseManager.setScore(gameScore);
        paused = true;
        Time.timeScale = 0f;
        pauseManager.ShowDeathMenu();
    }
    public void resume()
    {
        pauseManager.HidePauseMenu();
        pauseManager.HideDeathMenu();
        paused = false;
        Time.timeScale = 1f;
    }

    public void backToMenu()
    {
        resume();
        restartGameState();
        changeScene(0);
    }

    public void restartGame()
    {
        resume();
        restartGameState();
        changeScene(1);
    }

    private void restartGameState()
    {
        resume();
        activeBalls.Clear();
        paddle = null;
        godModeWall = null;
        levelStarted = false;
        blocksDestroyed = 0;
        blocksCurrent = 0;
        NextLvlButton.SetActive(false);
        itemSpawnProbability = 0;
        Physics.IgnoreLayerCollision(LayerMask.NameToLayer("Ball"), LayerMask.NameToLayer("Block"), false);
        UIBehaviour.Instance.drawBossUI(0, 0);
        nTotemsActive = 0;
        UIBehaviour.Instance.initInventory();
        isFinalBoss = false;
        lives = max_lives;
        drawLife();
        gameScore = 0;
        changeMusic("MainMenu");
    }

}
