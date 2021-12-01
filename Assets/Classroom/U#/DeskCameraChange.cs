
using UdonSharp;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;

public class DeskCameraChange : UdonSharpBehaviour
{
    public GameObject cam1;
    public GameObject cam2;
    
    void OnTriggerEnter(Collider other) {
        if(other.tag == "chair"){
            if(Input.GetButtonDown("1key")){
                cam1.SetActive(true);
                cam2.SetActive(false);
            }
            else if(Input.GetButtonDown("2key")){
                cam1.SetActive(false);
                cam2.SetActive(true);
            }
            
        }
    }

}