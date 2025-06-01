using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.EventSystems;
using TMPro; // Necesario para InputField TextMeshPro
using System.Text; // Necesario para StringBuilder
using UnityEngine.UI;
using System.Collections.Generic;
public class MainMenuManager : MonoBehaviour // O UIManager si es un script aparte
{
    // --- Referencias UI ---
    public GameObject mainButtonsGroup; // Asigna el objeto MainButtonsGroup en el Inspector
    public GameObject highScoresPanel;   // Asigna el HighScoresPanel en el Inspector
    public GameObject playerNamePanel;   // Asigna el PlayerNamePanel en el Inspector

    public TMP_InputField playerNameInput; // Asigna el PlayerNameInputField en el Inspector
    public Button playGameButton;          // Asigna el PlayGameButton en el Inspector
    public TextMeshProUGUI rankingDisplay; // Asigna el RankingText dentro del HighScoresPanel

    public GameObject splashTextObject;
    public GameObject title3DObject;

    // --- Sonidos (mantén tus variables de audio) ---
    public AudioClip hoverSound;
    public AudioClip clickSound;
    public AudioClip selectSound;
    private AudioSource uiAudioSource;

    void Awake()
    {
        // Busca el AudioSource en este mismo GameObject o en el UIManager
        uiAudioSource = GetComponent<AudioSource>();
        if (uiAudioSource == null)
        {
            uiAudioSource = GameObject.Find("UIManager")?.GetComponent<AudioSource>();
            if (uiAudioSource == null)
            {
                Debug.LogWarning("No AudioSource found for UI sounds. Please add one to this GameObject or a UIManager object.");
            }
        }

        // Carga los sonidos desde la carpeta Resources
        if (hoverSound == null)
        {
            hoverSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); 
        }
        if (clickSound == null)
        {
            clickSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); 
        }
        if (selectSound == null)
        {
            selectSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); 
        }

        if (hoverSound == null) Debug.LogWarning("Hover sound not found in Resources/Audio/UI/. Check filename.");
        if (clickSound == null) Debug.LogWarning("Click sound not found in Resources/Audio/UI/. Check filename.");
        if (selectSound == null) Debug.LogWarning("Select sound not found in Resources/Audio/UI/. Check filename.");

        // Si el InputField no tiene nombre guardado, inicializarlo
        if (PlayerPrefs.HasKey("LastPlayerName"))
        {
            playerNameInput.text = PlayerPrefs.GetString("LastPlayerName");
        }
        else
        {
            playerNameInput.text = "";
        }

        // Añadir un listener al InputField para habilitar/deshabilitar el botón Play
        playerNameInput.onValueChanged.AddListener(OnPlayerNameChanged);

        // Asegurarse de que el botón Play esté deshabilitado al inicio si el campo está vacío
        OnPlayerNameChanged(playerNameInput.text);
    }
    void Start()
    {
        // Asegurarse de que los paneles estén en el estado correcto al inicio
        mainButtonsGroup.SetActive(true);
        highScoresPanel.SetActive(false);
        playerNamePanel.SetActive(false);

        if (splashTextObject != null) splashTextObject.SetActive(true);
        if (title3DObject != null) title3DObject.SetActive(true);
    }

    // --- Funciones para mostrar/ocultar paneles ---

    private void SetMainMenuElementsActive(bool active)
    {
        mainButtonsGroup.SetActive(active);
        if (splashTextObject != null) splashTextObject.SetActive(active);
        if (title3DObject != null) title3DObject.SetActive(active);
    }

    // Cuando se hace clic en "Start Game" del menú principal
    public void ShowPlayerNameInput()
    {
        mainButtonsGroup.SetActive(false); // Ocultar botones principales
        highScoresPanel.SetActive(false); // Asegurarse de que High Score Panel esté oculto
        playerNamePanel.SetActive(true);  // Mostrar el formulario de nombre

        playerNameInput.Select(); // Opcional: enfocar el InputField automáticamente
        PlayClickSound(); // Reproducir sonido de clic
    }

    // Cuando se hace clic en "Play!" en el formulario de nombre
    public void StartGame()
    {
        string currentPlayerName = playerNameInput.text.Trim();
        if (string.IsNullOrWhiteSpace(currentPlayerName))
        {
            // Esto no debería pasar si el botón está deshabilitado, pero es una protección
            Debug.LogWarning("Player name cannot be empty!");
            return;
        }

        PlayerPrefs.SetString("LastPlayerName", currentPlayerName);
        PlayerPrefs.Save();

        Debug.Log($"Starting game with player: {currentPlayerName}");
        PlayClickSound(); // Sonido de clic
        GameManager.Instance.changeScene(1);
        //SceneManager.LoadScene("Level1"); // Cargar la escena del juego
    }

    // Cuando se hace clic en "Cancel" en el formulario de nombre
    public void CancelPlayerNameInput()
    {
        playerNamePanel.SetActive(false);   // Ocultar formulario de nombre
        SetMainMenuElementsActive(true); // Mostrar botones principales
        PlayClickSound(); // Sonido de clic
    }

    // Método que se llama cuando el texto del InputField cambia
    private void OnPlayerNameChanged(string newText)
    {
        // Habilitar el botón Play solo si el campo de nombre no está vacío
        playGameButton.interactable = !string.IsNullOrWhiteSpace(newText);
    }

    // --- Funciones para High Scores ---

    public void ShowHighScores()
    {
        SetMainMenuElementsActive(false); // Ocultar botones principales y otros elementos del menú
        playerNamePanel.SetActive(false); // Asegurarse de que PlayerName Panel esté oculto
        highScoresPanel.SetActive(true);   // Mostrar el panel de High Scores

        UpdateHighScoresDisplay();
        PlayClickSound();
    }

    public void HideHighScores()
    {
        highScoresPanel.SetActive(false);   // Ocultar panel de High Scores
        SetMainMenuElementsActive(true); // Mostrar botones principales y otros elementos del menú
        PlayClickSound(); // Sonido de clic
    }

    private void UpdateHighScoresDisplay()
    {
        // ... (Tu código existente para actualizar el rankingDisplay) ...
        if (rankingDisplay == null || HighScoreManager.Instance == null)
        {
            Debug.LogError("Ranking Display Text or HighScoreManager instance is missing!");
            return;
        }

        StringBuilder sb = new StringBuilder();

        List<ScoreEntry> scores = HighScoreManager.Instance.CurrentHighScores;

        if (scores.Count == 0)
        {
            sb.AppendLine("\nNo high scores yet!");
            sb.AppendLine("Play a game to set one!");
        }
        else
        {
            for (int i = 0; i < scores.Count; i++)
            {
                sb.AppendLine($"{i + 1}. {scores[i].playerName} - {scores[i].score}");
            }
        }

        rankingDisplay.text = sb.ToString();
    }

    // --- Funciones de Sonido (las que ya tenías) ---
    public void PlayHoverSound()
    {
        if (uiAudioSource != null && hoverSound != null)
        {
            uiAudioSource.PlayOneShot(hoverSound);
        }
    }

    public void PlayClickSound()
    {
        if (uiAudioSource != null && clickSound != null)
        {
            uiAudioSource.PlayOneShot(clickSound);
        }
    }

    public void PlaySelectSound()
    {
        if (uiAudioSource != null && selectSound != null)
        {
            uiAudioSource.PlayOneShot(selectSound);
        }
    }

    public void QuitGame()
    {
        Debug.Log("Quitting Game...");
        PlayClickSound(); // Sonido de clic
        Application.Quit();
        #if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
        #endif
    }
}