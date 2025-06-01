using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections; // Para Coroutines

public class CreditsManager : MonoBehaviour
{
    // --- Referencias a los GameObjects y sus Animators ---
    [Header("Animated Objects")]
    public GameObject titleObject;
    public Animator titleAnimator;

    public GameObject thanksTextObject;
    public Animator thanksTextAnimator;
    public GameObject thanksGifObject; // El GameObject que contiene tu primer GIF
    public Animator thanksGifAnimator;

    public GameObject oscarNameObject;
    public Animator oscarNameAnimator;
    public GameObject oscarGifObject; // El GameObject que contiene tu segundo GIF
    public Animator oscarGifAnimator;

    public GameObject arnauNameObject;
    public Animator arnauNameAnimator;
    public GameObject arnauGifObject; // El GameObject que contiene tu tercer GIF
    public Animator arnauGifAnimator;

    public GameObject finalMessageObject; // El GameObject de tu texto final
    public Animator finalMessageAnimator;

    public GameObject fadePanelObject;
    public Animator fadePanelAnimator;

    // --- Clips de Audio ---
    [Header("Audio Clips")]
    public AudioClip enterSound; // Sonido cuando el texto/nombre aparece
    public AudioClip finalSound; // Sonido al final

    // --- Tiempos de Retraso ---
    [Header("Timing")]
    public float titleEnterDelay = 0.5f;
    public float titleStayDuration = 3f;
    public float thanksEnterDelay = 1f; // Retraso después de que el título se detiene
    public float thanksStayDuration = 15f; // Duración antes de que Thanks y su GIF se oculten
    public float namesEnterDelay = 1f; // Retraso después de que Thanks se oculta
    public float namesStayDuration = 10f; // Duración antes de que los nombres se oculten
    public float finalMessageEnterDelay = 1f; // Retraso después de que los nombres se ocultan
    public float finalMessageStayDuration = 10f; // Duración antes de volver al menú

    private AudioSource _audioSource; // Para reproducir sonidos puntuales

    void Awake()
    {
        _audioSource = GetComponent<AudioSource>();
        if (_audioSource == null)
        {
            _audioSource = gameObject.AddComponent<AudioSource>();
        }
    }

    void Start()
    {
        // Asegurarse de que todos los objetos estén inicialmente inactivos o en su posición de inicio
        SetObjectActive(titleObject, false);
        SetObjectActive(thanksTextObject, false);
        SetObjectActive(thanksGifObject, false);
        SetObjectActive(oscarNameObject, false);
        SetObjectActive(oscarGifObject, false);
        SetObjectActive(arnauNameObject, false);
        SetObjectActive(arnauGifObject, false);
        SetObjectActive(finalMessageObject, false);

        StartCoroutine(CreditsSequence());
    }

    private void SetObjectActive(GameObject obj, bool active)
    {
        if (obj != null)
        {
            obj.SetActive(active);
        }
    }

    IEnumerator CreditsSequence()
    {
        yield return new WaitForSeconds(titleEnterDelay); // Retraso inicial

        // --- Fase 1: Título ---
        SetObjectActive(titleObject, true);
        if (titleAnimator != null)
        {
            titleAnimator.SetTrigger("Enter"); // Activa la animación de entrada del título
        }
        yield return new WaitForSeconds(titleAnimator.GetCurrentAnimatorClipInfo(0)[0].clip.length); // Espera a que termine la animación
        if (_audioSource != null && enterSound != null) _audioSource.PlayOneShot(enterSound);
        yield return new WaitForSeconds(titleStayDuration); // Espera antes de la siguiente fase

        // --- Fase 2: Thanks y GIF 1 ---
        SetObjectActive(thanksTextObject, true);
        SetObjectActive(thanksGifObject, true);
        if (thanksTextAnimator != null) thanksTextAnimator.SetTrigger("Enter");
        if (thanksGifAnimator != null) thanksGifAnimator.SetTrigger("Enter");
        yield return new WaitForSeconds(thanksTextAnimator.GetCurrentAnimatorClipInfo(0)[0].clip.length); // Espera a que termine la animación
        if (_audioSource != null && enterSound != null) _audioSource.PlayOneShot(enterSound);
        yield return new WaitForSeconds(thanksStayDuration); // Espera antes de ocultarlos

        // Ocultar Thanks y GIF 1
        if (titleAnimator != null) titleAnimator.SetTrigger("Exit");
        if (thanksTextAnimator != null) thanksTextAnimator.SetTrigger("Exit");
        if (thanksGifAnimator != null) thanksGifAnimator.SetTrigger("Exit");
        // Esperar a que salgan de pantalla (puedes ajustar el tiempo si la animación de salida es larga)
        yield return new WaitForSeconds(3f); // Ejemplo de tiempo para que salgan
        SetObjectActive(thanksTextObject, false);
        SetObjectActive(thanksGifObject, false);

        yield return new WaitForSeconds(namesEnterDelay); // Retraso antes de los nombres

        // --- Fase 3: Nombres Oscar y GIF 2 ---
        SetObjectActive(oscarNameObject, true);
        SetObjectActive(oscarGifObject, true);
        if (oscarNameAnimator != null) oscarNameAnimator.SetTrigger("Enter");
        if (oscarGifAnimator != null) oscarGifAnimator.SetTrigger("Enter");
        yield return new WaitForSeconds(oscarNameAnimator.GetCurrentAnimatorClipInfo(0)[0].clip.length);
        if (_audioSource != null && enterSound != null) _audioSource.PlayOneShot(enterSound);
        yield return new WaitForSeconds(1f); // Pequeña pausa antes de Arnau (ajusta si quieres)

        // --- Fase 4: Nombre Arnau y GIF 3 ---
        SetObjectActive(arnauNameObject, true);
        SetObjectActive(arnauGifObject, true);
        if (arnauNameAnimator != null) arnauNameAnimator.SetTrigger("Enter");
        if (arnauGifAnimator != null) arnauGifAnimator.SetTrigger("Enter");
        yield return new WaitForSeconds(arnauNameAnimator.GetCurrentAnimatorClipInfo(0)[0].clip.length);
        if (_audioSource != null && enterSound != null) _audioSource.PlayOneShot(enterSound);
        yield return new WaitForSeconds(namesStayDuration); // Espera antes de ocultarlos

        // Ocultar Oscar, Arnau y Gifs
        if (oscarNameAnimator != null) oscarNameAnimator.SetTrigger("Exit");
        if (oscarGifAnimator != null) oscarGifAnimator.SetTrigger("Exit");
        if (arnauNameAnimator != null) arnauNameAnimator.SetTrigger("Exit");
        if (arnauGifAnimator != null) arnauGifAnimator.SetTrigger("Exit");
        yield return new WaitForSeconds(3f); // Tiempo para que salgan
        SetObjectActive(oscarNameObject, false);
        SetObjectActive(oscarGifObject, false);
        SetObjectActive(arnauNameObject, false);
        SetObjectActive(arnauGifObject, false);

        yield return new WaitForSeconds(finalMessageEnterDelay); // Retraso antes del mensaje final

        // --- Fase 5: Mensaje Final ---
        SetObjectActive(finalMessageObject, true);
        if (finalMessageAnimator != null) finalMessageAnimator.SetTrigger("Enter");
        yield return new WaitForSeconds(finalMessageAnimator.GetCurrentAnimatorClipInfo(0)[0].clip.length);
        if (_audioSource != null && finalSound != null) _audioSource.PlayOneShot(finalSound); // Sonido final
        yield return new WaitForSeconds(finalMessageStayDuration); // Espera antes de volver al menú

        // --- Fase 6: Animación de Oscurecimiento ---
        if (fadePanelObject != null)
        {
            fadePanelObject.SetActive(true); // Activa el Canvas/Panel que contiene la FadeImage
        }

        if (fadePanelAnimator != null)
        {
            fadePanelAnimator.SetTrigger("DoFade"); // Dispara la animación de oscurecimiento
            // Espera a que la animación de oscurecimiento termine
            yield return new WaitForSeconds(fadePanelAnimator.GetCurrentAnimatorStateInfo(0).length);
        }
        else
        {
            // Si no hay Animator, espera un tiempo fijo para simular el fade
            yield return new WaitForSeconds(1.5f); // Tiempo de espera si no hay animación
        }


        // --- Fase Final: Volver al menú ---
        EndGameAndReturnToMenu();
    }

    private void EndGameAndReturnToMenu()
    {
        // Llama al GameManager para guardar la puntuación (si es la última escena)
        // y para reiniciar el estado del juego para una nueva partida.
        // Asegúrate de que HighScoreManager y GameManager son Singletons y persisten.
        if (GameManager.Instance != null)
        {
            GameManager.Instance.changeScene(0);
        }
        else
        {
            // Si GameManager no persiste o no está en la escena, simplemente vuelve al menú.
            Debug.LogWarning("GameManager instance not found. Returning to MainMenu directly.");
            SceneManager.LoadScene("MainMenu");
        }
    }
}