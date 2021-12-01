
using UdonSharp;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;

public class testbutton : UdonSharpBehaviour
{
    public bool toggle;
    public GameObject gameObject;


    public override void Interact() {
        if(toggle){
            toggle = false;
            gameObject.SetActive(false);
        }else{
            gameObject.SetActive(true);
            toggle = true;
        }
    }

    
}
