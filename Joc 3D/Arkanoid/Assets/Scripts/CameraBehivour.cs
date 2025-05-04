using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraBehivour : MonoBehaviour
{
    public Transform target;          // Centro del nivel
    public float duration;       // Cuánto tiempo dura la animación
    public float speed;         // Velocidad de rotación
    public float dist;      // Distancia al centro

    private float _timer;
    private bool _orbiting;

    private Vector3 _offset;

    // Start is called before the first frame update
    void Start()
    {
        _timer = 0;
        _orbiting = true;
        _offset = (transform.position - target.position).normalized * dist;
        transform.position = target.position + _offset;
        transform.LookAt(target);
    }

    // Update is called once per frame
    void Update()
    {
        if (_orbiting)
        {
            _timer += Time.deltaTime;

            transform.RotateAround(target.position, Vector3.down, speed * Time.deltaTime);
            transform.LookAt(target);

            if (_timer >= duration)
            {
                _orbiting = false;
            }
        }
    }
}
