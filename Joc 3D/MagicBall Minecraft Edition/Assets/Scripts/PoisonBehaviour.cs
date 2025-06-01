using System.Collections;
using UnityEngine;

public class PoisonZone : MonoBehaviour
{
    public float duration;

    private void Start()
    {
        duration = 45;
        StartCoroutine(SelfDestructAfterTime());
    }

    private IEnumerator SelfDestructAfterTime()
    {
        yield return new WaitForSeconds(duration);
        Destroy(gameObject);
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Paddle"))
        {
            PoisonManager.Instance.PlayerEnteredZone();
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Paddle"))
        {
            PoisonManager.Instance.PlayerExitedZone();
        }
    }
}
