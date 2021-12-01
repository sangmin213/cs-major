
using UdonSharp;
using UnityEngine;
using UnityEngine.UI;
using VRC.SDKBase;
using VRC.Udon;
using UnityEngine;
using System.Collections;
using UnityEngine.Networking;

namespace childofimage.tutorial
{
    class MyBehaviour : UdonSharpBehaviour{

        public RawImage image;
        public InputField inputField;

        
        void Start()
        {
            GetTexture();
        }

        IEnumerator GetTexture()
        {
            //string url=GameObject.Find("urlField").GetComponent<InputURL>().Update();
            UnityWebRequest www = UnityWebRequestTexture.GetTexture(inputField.text);
            yield return www.SendWebRequest();
            if (www.result != UnityWebRequest.Result.Success)
            {
                Debug.Log(www.error);
            }
            else
            {
                image.texture = ((DownloadHandlerTexture)www.downloadHandler).texture;
            }
        }
    }
}

