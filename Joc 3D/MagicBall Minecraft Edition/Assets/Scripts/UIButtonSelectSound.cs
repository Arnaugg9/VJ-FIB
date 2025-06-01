using UnityEngine;
using UnityEngine.EventSystems;

public class UIButtonSelectSound : MonoBehaviour, ISelectHandler
{
    private MainMenuManager mainMenuManager; // O UIAudioManager

    void Awake()
    {
        // Busca la instancia de tu MainMenuManager (o UIAudioManager) en la escena
        mainMenuManager = FindObjectOfType<MainMenuManager>(); // Ajusta a UIAudioManager si ese es el nombre
        if (mainMenuManager == null)
        {
            Debug.LogError("MainMenuManager (or UIAudioManager) not found in scene. Please ensure it's present and named correctly.");
        }
    }

    // Se llama cuando el botón es seleccionado (por teclado, ratón, etc.)
    public void OnSelect(BaseEventData eventData)
    {
        if (mainMenuManager != null)
        {
            mainMenuManager.PlaySelectSound();
        }
    }
}