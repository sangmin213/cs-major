using UdonSharp;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;

//make Search panel (class) 

//make text area (class)

//Search book data set (method) - public

//

public class bookshelf : UdonSharpBehaviour
{
    public bool toggle;
    public GameObject gameObject;

    
    public override void Interact() {       
        if(toggle){
            toggle = false;
            //frame.setVisible(False);
        }else{
            toggle = true;
            //frame.setVisible(False);
        }
    }

    
}
