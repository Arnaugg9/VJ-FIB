using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System.Collections;

public class RandomBackgroundChanger : MonoBehaviour
{
    [Header("UI Settings")]
    [Tooltip("Asigna aquí el primer componente Image para el fondo.")]
    public Image imageSlotA;
    [Tooltip("Asigna aquí el segundo componente Image para el fondo.")]
    public Image imageSlotB;

    [Header("Background Images")]
    public List<Sprite> backgroundImages = new List<Sprite>();

    [Header("Change Timing")]
    [Tooltip("Minimum time in seconds before a change.")]
    public float minTimeBetweenChanges = 10f;
    [Tooltip("Maximum time in seconds before a change.")]
    public float maxTimeBetweenChanges = 30f;
    [Tooltip("Duration of the cross-fade transition in seconds.")]
    public float fadeDuration = 1.0f;

    private Image currentActiveSlot;
    private Image nextSlot;
    private Coroutine changeCoroutine;

    void Start()
    {
        if (imageSlotA == null || imageSlotB == null)
        {
            Debug.LogError("RandomBackgroundChanger: Uno o ambos slots de Image no han sido asignados.");
            enabled = false;
            return;
        }

        if (backgroundImages.Count == 0)
        {
            Debug.LogError("RandomBackgroundChanger: No hay imágenes de fondo asignadas en la lista.");
            enabled = false;
            return;
        }

        // Inicializar slots
        currentActiveSlot = imageSlotA;
        nextSlot = imageSlotB;

        // Establecer la primera imagen aleatoria en el slot activo, sin fundido
        SetSpriteForSlot(currentActiveSlot, GetRandomSprite(null));
        SetAlpha(currentActiveSlot, 1f); // Completamente visible
        SetAlpha(nextSlot, 0f);         // Completamente transparente

        if (backgroundImages.Count > 1)
        {
            changeCoroutine = StartCoroutine(TryChangeBackgroundPeriodically());
        }
    }

    Sprite GetRandomSprite(Sprite spriteToAvoid)
    {
        if (backgroundImages.Count == 0) return null;
        if (backgroundImages.Count == 1) return backgroundImages[0];

        List<Sprite> possibleSprites = new List<Sprite>(backgroundImages);
        if (spriteToAvoid != null)
        {
            possibleSprites.Remove(spriteToAvoid);
            if (possibleSprites.Count == 0) // Si al quitarla no quedan, usa la lista completa
            {
                possibleSprites.AddRange(backgroundImages);
            }
        }
        return possibleSprites[Random.Range(0, possibleSprites.Count)];
    }

    void SetSpriteForSlot(Image slot, Sprite sprite)
    {
        if (slot != null && sprite != null)
        {
            slot.sprite = sprite;
        }
    }

    void SetAlpha(Image slot, float alpha)
    {
        if (slot != null)
        {
            Color color = slot.color;
            color.a = Mathf.Clamp01(alpha);
            slot.color = color;
        }
    }

    IEnumerator TryChangeBackgroundPeriodically()
    {
        while (true)
        {
            float waitTime = Random.Range(minTimeBetweenChanges, maxTimeBetweenChanges);
            yield return new WaitForSeconds(waitTime);

            // Cargar el nuevo sprite en el slot que está actualmente oculto (nextSlot)
            Sprite spriteToAvoid = currentActiveSlot.sprite;
            SetSpriteForSlot(nextSlot, GetRandomSprite(spriteToAvoid));

            // Iniciar la transición de fundido cruzado
            if (fadeDuration > 0)
            {
                yield return StartCoroutine(CrossFade(currentActiveSlot, nextSlot, fadeDuration));
            }
            else // Cambio instantáneo si no hay duración de fundido
            {
                SetAlpha(currentActiveSlot, 0f);
                SetAlpha(nextSlot, 1f);
            }

            // Intercambiar los roles de los slots para la próxima transición
            Image temp = currentActiveSlot;
            currentActiveSlot = nextSlot;
            nextSlot = temp;
        }
    }

    IEnumerator CrossFade(Image fadingOutSlot, Image fadingInSlot, float duration)
    {
        if (duration <= 0) // Manejar duración cero o negativa para cambio instantáneo
        {
            SetAlpha(fadingOutSlot, 0f);
            SetAlpha(fadingInSlot, 1f);
            yield break;
        }

        float timer = 0f;
        // No es necesario obtener startAlpha aquí, ya que asumimos que fadingOutSlot.a es 1 y fadingInSlot.a es 0

        while (timer < duration)
        {
            timer += Time.deltaTime;
            float progress = timer / duration;

            SetAlpha(fadingOutSlot, 1f - progress); // De 1 a 0
            SetAlpha(fadingInSlot, progress);      // De 0 a 1

            yield return null;
        }

        // Asegurar que los alfas finales sean exactos
        SetAlpha(fadingOutSlot, 0f);
        SetAlpha(fadingInSlot, 1f);
    }

    void OnDisable()
    {
        if (changeCoroutine != null)
        {
            StopCoroutine(changeCoroutine);
            changeCoroutine = null;
        }
    }

    void OnEnable()
    {
        if (imageSlotA != null && imageSlotB != null && backgroundImages.Count > 1 && changeCoroutine == null)
        {
            if (Application.isPlaying && Time.timeSinceLevelLoad > 0.1f)
            {
                // Si se reactiva, hay que reestablecer el estado visual correcto
                // Asumimos que currentActiveSlot y nextSlot mantienen sus referencias
                // y que sus alfas podrían no ser los correctos.
                // Lo más simple es reiniciar la lógica de Start o una subrutina.
                // Por ahora, simplemente reiniciamos la corrutina de cambio.
                // Esto podría causar un salto si los alfas no estaban 0 y 1.
                // Una lógica más robusta aquí podría ser necesaria si se desactiva/activa frecuentemente.
                changeCoroutine = StartCoroutine(TryChangeBackgroundPeriodically());
            }
            else if (!Application.isPlaying) // Para evitar error en editor si se habilita antes de Start
            {
                // No hacer nada o prepararse para Start
            }
        }
    }
}