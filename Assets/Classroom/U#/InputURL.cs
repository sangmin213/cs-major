
using UdonSharp;
using UnityEngine;
using UnityEngine;
using VRC.SDKBase;
using VRC.Udon;
using System.Collections;
using TMPro;


public class InputURL : UdonSharpBehaviour
{
    public GameObject inputfield;




    public override void Interact() {
        //inputfield.text=$"Value changed : ";
    }

    /*
    public GameObject inputfield;
    string Text = inputfield.GetComponent<TMP_InputField>().text;
    //public TextMeshProUGUI urltext;

    //TextMeshProUGUI
    //public InputField


    public void OnValueChangedEvent(string str)
    {
        //urltext.OnPointerEnter(OnEndEvent(str));
        inputfield.text= $"Value changed : {str}";
    }

    public void OnEndEvent(string str)
    {
        inputfield.text= $"Value changed : {str}";
    }

    public void OnSelectEvent(string str)
    {
        inputfield.text= $"Value changed : {str}";
    }

    public void OnDeselectEvent(string str)
    {
        inputfield.text= $"Value changed : {str}";
    }
    
    void LockInput(InputField input) {
        if (input.text.Length > 0) { 
            Debug.Log("Text has been entered"); 
        } 
        else if (input.text.Length == 0) { 
            Debug.Log("Main Input Empty"); 
        }
    } 
    public void Start() 
    { 
        urlField.OnSubmit().AddListener(delegate{LockInput(urlField);}); 
    }

    public string Update() {
        urlField.OnSubmit().AddListener(delegate{LockInput(urlField);});             

        Debug.Log(urlField.text);
        
        return urlField.text;
    }
    */
    
}
