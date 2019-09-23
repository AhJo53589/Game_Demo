using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


public class MainMenu : MonoBehaviour
{
    public Button m_Button_Play;
    public Button m_Button_Quit;
    public Button[] m_Button_Level;
    public Slider m_Slider_Anim;

    
	// Use this for initialization
	void Start ()
    {
        m_Button_Play.onClick.AddListener(OnButtonPlayClick);
        m_Button_Quit.onClick.AddListener(OnButtonQuitClick);

        foreach (Button btn in m_Button_Level)
        {
            btn.gameObject.SetActive(false);
            btn.onClick.AddListener(() =>
            {
                //string str = btn.transform.Find("Text").GetComponent<Text>().text.ToString();
                string str = btn.GetComponentInChildren<Text>().text.ToString();
                //string str = "Level 01";
                JumpScene(str);
            });
        }
	}
	
	// Update is called once per frame
	void Update ()
    {
    }

    void OnButtonPlayClick()
    {
        StartCoroutine(SliderRun());
        Debug.Log("OnButtonPlayClick");
    }

    void OnButtonQuitClick()
    {
        Application.Quit();
    }

    void OnButtonLevel01Click()
    {
        JumpScene("Level 01");
    }

    void JumpScene(string sceneName)
    {
        SceneManager.LoadScene(sceneName, LoadSceneMode.Single);
    }


    IEnumerator SliderRun()
    {
        while (true)
        {
            if (m_Slider_Anim.value < 10)
            {
                m_Slider_Anim.value += Time.deltaTime;
            }
            else
            {
                Debug.Log("SliderRun");
                m_Button_Level[0].gameObject.SetActive(true);
                yield break;
            }
            yield return 0;
        }
    }
}
