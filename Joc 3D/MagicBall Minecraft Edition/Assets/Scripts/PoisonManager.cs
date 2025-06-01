using UnityEngine;

public class PoisonManager : MonoBehaviour
{
    public static PoisonManager Instance;

    public float timeToHurt = 2f;
    public float recoverySpeed = 0.25f;
    private float timer;

    private int poisonZonesTouching = 0;

    private AudioSource audioSource;
    public AudioClip posionClip;

    private void Awake()
    {
        if (Instance == null) Instance = this;
        else Destroy(gameObject);
    }

    private void Start()
    {
        audioSource = GetComponent<AudioSource>();
        audioSource.clip = posionClip;
        audioSource.loop = true;
        audioSource.playOnAwake = false;
        audioSource.volume = 0.9f;
        timer = timeToHurt;
    }

    private void Update()
    {
        if (poisonZonesTouching > 0)
        {
            timer -= Time.deltaTime;
            if (timer <= 0f)
            {
                GameManager.Instance.loseLife();
                timer = timeToHurt;
            }

            if (!audioSource.isPlaying) audioSource.Play();

            audioSource.pitch = Random.Range(0.3f, 1);
        }
        else
        {
            if (timer < timeToHurt)
            {
                timer += Time.deltaTime * recoverySpeed;
                if (timer > timeToHurt)
                    timer = timeToHurt;
            }

            if (audioSource.isPlaying) audioSource.Stop();
        }
    }

    public void PlayerEnteredZone()
    {
        poisonZonesTouching++;
    }

    public void PlayerExitedZone()
    {
        poisonZonesTouching--;
        if (poisonZonesTouching < 0) poisonZonesTouching = 0;
    }
}
