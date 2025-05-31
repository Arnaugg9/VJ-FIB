using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class UIBehaviour : MonoBehaviour
{
    public static UIBehaviour Instance { get; private set; }

    public GameObject itemBalls;
    public GameObject itemTotem;
    public GameObject itemBig;
    public GameObject itemSmall;
    public GameObject itemMagnet;
    public GameObject itemPower;
    public GameObject itemShoot;

    //UI del boss
    public Texture fullEnemyheart;
    public Texture emptyHeart;
    public List<GameObject> enemyLifeSlots;

    public AudioClip breakItemClip;

    private void Awake()
    {
        if (Instance != null)
        {
            Destroy(gameObject);
            return;
        }
        Instance = this;
        DontDestroyOnLoad(gameObject);
        GameManager.Instance.Game_UI = gameObject;
    }

    // Start is called before the first frame update
    void Start()
    {
        initInventory();
        drawBossUI(0, 0);
    }

    public void initInventory()
    {
        itemBalls.GetComponentInChildren<TextMeshProUGUI>().text = GameManager.Instance.activeBalls.Count.ToString();

        itemBig.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemBig.SetActive(false);

        itemSmall.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemSmall.SetActive(false);

        itemMagnet.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemMagnet.SetActive(false);

        itemPower.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemPower.SetActive(false);

        itemShoot.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemShoot.SetActive(false);

        itemTotem.GetComponentInChildren<TextMeshProUGUI>().text = "0";
        itemTotem.SetActive(false);
    }

    public void updateUI(string item, int value)
    {
        if (item == "ball") itemBalls.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "big") itemBig.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "small") itemSmall.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "magnet") itemMagnet.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "power") itemPower.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "shoot") itemShoot.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else if (item == "totem") itemTotem.GetComponentInChildren<TextMeshProUGUI>().text = value.ToString();
        else Debug.LogError("Error when changing UI item State");
    }

    public void ch_stateItemUI(string item, bool state)
    {
        if (item == "big") itemBig.SetActive(state);
        else if (item == "small") itemSmall.SetActive(state);
        else if (item == "magnet") itemMagnet.SetActive(state);
        else if (item == "power") itemPower.SetActive(state);
        else if (item == "shoot") itemShoot.SetActive(state);
        else if (item == "totem") itemTotem.SetActive(state);
        else Debug.LogError("Error when changing UI item State");

        if (!state) AudioSource.PlayClipAtPoint(breakItemClip, Camera.main.transform.position);
    }

    public void drawBossUI (int health, int max_health)
    {
        int idx = 0;
        for (; idx < health; ++idx)
        {
            enemyLifeSlots[idx].SetActive(true);
            enemyLifeSlots[idx].gameObject.GetComponent<UnityEngine.UI.RawImage>().texture = fullEnemyheart;
        }
        for (;idx < max_health; ++idx)
        {
            enemyLifeSlots[idx].SetActive(true);
            enemyLifeSlots[idx].gameObject.GetComponent<UnityEngine.UI.RawImage>().texture = emptyHeart;
        }
        for (;idx < 10; ++idx)
        {
            enemyLifeSlots[idx].SetActive(false);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
