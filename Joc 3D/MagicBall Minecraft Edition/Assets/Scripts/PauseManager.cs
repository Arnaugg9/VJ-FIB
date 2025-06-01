using TMPro;
using UnityEngine;

public class PauseManager : MonoBehaviour
{
    public GameObject pauseMenu;
    public GameObject deathMenu;
    public TextMeshProUGUI scoreText;
    private AudioSource audioSource;
    private AudioClip buttonSound;

    private void Awake()
    {
        if (GameManager.Instance.pauseManager == null) GameManager.Instance.pauseManager = this;
    }

    void Start()
    {
        audioSource = GetComponent<AudioSource>();
        buttonSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo");
        pauseMenu.SetActive(false);
        deathMenu.SetActive(false);
    }

    public void ShowPauseMenu()
    {
        pauseMenu.SetActive(true);
    }

    public void HidePauseMenu()
    {
        pauseMenu.SetActive(false);
    }

    public void ShowDeathMenu()
    {
        deathMenu.SetActive(true);
    }

    public void HideDeathMenu()
    {
        deathMenu.SetActive(false);
    }

    public void setScore(int score)
    {
        scoreText.text = " " + score.ToString();
    }

    public void OnResumeButton()
    {
        GameManager.Instance.resume(); 
    }

    public void OnBackToMenuButton()
    {
        GameManager.Instance.backToMenu();
    }

    public void OnRestartButton()
    {
        GameManager.Instance.restartGame();
    }

    public void PlaySound()
    {
        if (audioSource != null && buttonSound != null)
        {
            audioSource.PlayOneShot(buttonSound);
        }
    }

}

