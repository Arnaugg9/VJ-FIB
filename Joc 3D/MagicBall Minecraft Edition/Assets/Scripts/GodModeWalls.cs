using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GodModeWalls : MonoBehaviour
{
    private void Awake()
    {
        GameManager.Instance.godModeWall = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
