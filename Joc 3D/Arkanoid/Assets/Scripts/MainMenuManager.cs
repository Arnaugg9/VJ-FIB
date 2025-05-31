using UnityEngine;
using UnityEngine.EventSystems; // Necesario para la interfaz IPointerEnterHandler y IPointerExitHandler

public class MainMenuManager : MonoBehaviour // O UIManager si es un script aparte
{
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
            // Si el AudioSource está en otro GameObject (e.g., UIManager), búscalo
            uiAudioSource = GameObject.Find("UIManager")?.GetComponent<AudioSource>();
            if (uiAudioSource == null)
            {
                Debug.LogWarning("No AudioSource found for UI sounds. Please add one to this GameObject or a UIManager object.");
            }
        }

        // Carga los sonidos desde la carpeta Resources
        if (hoverSound == null)
        {
            hoverSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); // Reemplaza con el nombre real de tu archivo
        }
        if (clickSound == null)
        {
            clickSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); // Reemplaza con el nombre real de tu archivo
        }
        if (selectSound == null)
        {
            selectSound = Resources.Load<AudioClip>("Audio/UI/Click_stereo"); // Reemplaza con el nombre real de tu archivo
        }

        // Asegúrate de que los sonidos se hayan cargado correctamente
        if (hoverSound == null) Debug.LogWarning("Hover sound not found in Resources/Audio/UI/. Check filename.");
        if (clickSound == null) Debug.LogWarning("Click sound not found in Resources/Audio/UI/. Check filename.");
        if (selectSound == null) Debug.LogWarning("Select sound not found in Resources/Audio/UI/. Check filename.");
    }

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

    // ... tus otras funciones de menú (StartGame, HowToPlay, etc.) ...
}