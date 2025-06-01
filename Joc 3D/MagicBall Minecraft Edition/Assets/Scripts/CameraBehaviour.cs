using UnityEngine;

public class CameraBehaviour : MonoBehaviour
{
    [Header("Pivote y timings")]
    public Transform target;          // Centro del nivel
    public float duration = 3f;       // Duracion de la animacion

    [Header("Marcador de posicion final")]
    public Transform endMarker;       // Empty que marca la posicion final

    private float _timer;
    private bool _orbiting;

    private Vector3 _startOffset;     // offset inicial (posicion - target)
    private Vector3 _endOffset;       // offset final (endMarker - target)

    void Start()
    {
        _timer = 0f;
        _orbiting = true;

        // Calcula offsets relativos al target
        _startOffset = transform.position - target.position;
        _endOffset = endMarker.position - target.position;

        // Asegura posicion inicial y mirada al centro
        transform.position = target.position + _startOffset;
        transform.LookAt(target);
    }

    void Update()
    {
        if (!_orbiting) return;

        _timer += Time.deltaTime;
        float t = Mathf.Clamp01(_timer / duration);

        // Interpola sobre la esfera para mantener la misma distancia
        Vector3 offset = Vector3.Slerp(_startOffset, _endOffset, t);

        transform.position = target.position + offset;
        transform.LookAt(target);

        if (_timer >= duration)
        {
            _orbiting = false;
            GameManager.Instance.levelStarted = true;
        }
    }
}
