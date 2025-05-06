using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonActions : MonoBehaviour
{
    public void gotoNextLvl()
    {
        GameManager.Instance.gotoNextLvl();
    }
}
