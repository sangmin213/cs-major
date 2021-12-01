using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace camchange.tutorial{
    public class NewBehaviourScript : MonoBehaviour
    {
        public GameObject cam1;
        public GameObject cam2;

        void Update()
        {
            if(Input.GetButtonDown("1key")){
                cam1.SetActive(true);
                cam2.SetActive(false);
            }
            else if(Input.GetButtonDown("2key")){
                cam1.SetActive(false);
                cam2.SetActive(true);
            }
        }
        
        // Start is called before the first frame update
        
    }


}

