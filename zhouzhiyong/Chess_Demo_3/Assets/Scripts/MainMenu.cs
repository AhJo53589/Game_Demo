using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


public class MainMenu : MonoBehaviour
{
    public Button m_Button_Play;
    public Button m_Button_Quit;
	// Use this for initialization
	void Start ()
    {
        m_Button_Play.onClick.AddListener(OnButtonPlayClick);
        m_Button_Quit.onClick.AddListener(OnButtonQuitClick);
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    void OnButtonPlayClick()
    {
        JumpScene("Game");
    }

    void OnButtonQuitClick()
    {
        Application.Quit();
    }

    void JumpScene(string sceneName)
    {
        SceneManager.LoadScene(sceneName, LoadSceneMode.Single);
    }
}
