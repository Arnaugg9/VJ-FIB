using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class SelectSound : MonoBehaviour, ISelectHandler
{
    public PauseManager pauseManager; 
    void Awake()
    {
        pauseManager = GameManager.Instance.pauseManager;
        if (pauseManager == null)
        {
            Debug.LogError("MainMenuManager (or UIAudioManager) not found in scene. Please ensure it's present and named correctly.");
        }
    }

    // Se llama cuando el botón es seleccionado (por teclado, ratón, etc.)
    public void OnSelect(BaseEventData eventData)
    {
        if (pauseManager != null)
        {
            pauseManager.PlaySound();
        }
    }
}
