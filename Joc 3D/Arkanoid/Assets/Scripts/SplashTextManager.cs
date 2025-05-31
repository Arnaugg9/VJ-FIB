using UnityEngine;
using TMPro; // Necesario para TextMeshPro
using System.Collections;
using System.Collections.Generic; // Para usar List

public class SplashTextManager : MonoBehaviour
{
    public TextMeshProUGUI splashTextUI; // Referencia al objeto de texto TextMeshPro
    [Tooltip("Minimum time in seconds before changing the splash text.")]
    public float minChangeTime = 8f;
    [Tooltip("Maximum time in seconds before changing the splash text.")]
    public float maxChangeTime = 18f;

    private List<string> splashPhrases = new List<string>()
    {
        "We break blocks, but with more style!",
        "It's not PvP, it's P-B! (Player vs. Blocks)",
        "Beware of Creepers... in ball form!",
        "Building a new legacy, block by block!",
        "Snow Golem or TNT? You decide!",
        "It's not Breakout, it's Blockout!",
        "Explosions guaranteed! (Unless you break the ball!)",
        "Now with 100% less zombies! (Probably)",
        "Where's my pickaxe? Oh, don't need it anymore!",
        "The only game where breaking is building!",
        "Careful, the ball is slime!",
        "Made with 100% dirt blocks! (Not recommended)",
        "More addictive than mining diamond!",
        "Not for Nether players!",
        "Almost as good as pumpkin pie!",
        "No villagers to trade with here!",
        "Lava is not a good friend for the ball!",
        "Get ready for mining... of blocks!",
        "Creeper, creeper, oh my ball!",
        "Redstone won't save you from this!",
        "A game so good, even Steve would play!",
        "Powered by potatoes!",
        "Watch out for Endermen, they might steal your ball!",
        "Breaking news: Blocks are breakable!",
        "Don't dig straight down!",
        "It's not a bug, it's a feature... of the ball!",
        "Unlock all the achievements!",
        "Crafted with love (and pixels)!",
        "Welcome to the blocky side of life!",
        "More fun than punching wood!",
        "Experience the block-breaking frenzy!",
        "The ultimate block-busting adventure!"
    };

    private Coroutine changeTextCoroutine;

    void Start()
    {
        if (splashTextUI == null)
        {
            Debug.LogError("Splash Text UI reference is missing on SplashTextManager!", this);
            enabled = false; // Deshabilita el script si no hay referencia
            return;
        }

        // Inicia la corrutina para cambiar el texto
        changeTextCoroutine = StartCoroutine(ChangeSplashTextRoutine());
    }

    IEnumerator ChangeSplashTextRoutine()
    {
        while (true) // Bucle infinito
        {
            // Selecciona una frase aleatoria
            string randomPhrase = GetRandomSplashPhrase();
            splashTextUI.text = randomPhrase;

            // Espera un tiempo aleatorio entre minChangeTime y maxChangeTime
            float waitTime = Random.Range(minChangeTime, maxChangeTime);
            yield return new WaitForSeconds(waitTime);
        }
    }

    string GetRandomSplashPhrase()
    {
        if (splashPhrases.Count == 0)
        {
            return "¡No hay frases para mostrar!";
        }
        int randomIndex = Random.Range(0, splashPhrases.Count);
        return splashPhrases[randomIndex];
    }

    // Opcional: Para detener la corrutina si el objeto se desactiva o destruye
    void OnDisable()
    {
        if (changeTextCoroutine != null)
        {
            StopCoroutine(changeTextCoroutine);
        }
    }
}