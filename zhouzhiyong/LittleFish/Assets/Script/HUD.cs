using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HUD : MonoBehaviour
{
	public Level level;

	public UnityEngine.UI.Text remainingText;
	public UnityEngine.UI.Text remainingSubtext;
	public UnityEngine.UI.Text targetText;
	public UnityEngine.UI.Text targetSubtext;
	public UnityEngine.UI.Text scoreText;
	public UnityEngine.UI.Image[] stars;

	private int starIdx = 0;
	private bool isGameOver = false;

	// Use this for initialization
	void Start () 
	{
		for (int i = 0; i < stars.Length; i++)
		{
			stars[i].enabled = (i == starIdx);
		}
	}
	
	// Update is called once per frame
	void Update () 
	{
		
	}

	public void SetScore(int score)
	{
		scoreText.text = score.ToString();

		starIdx = 0;
		starIdx += (score >= level.score1Star) ? 1 : 0;
		starIdx += (score >= level.score2Star) ? 1 : 0;
		starIdx += (score >= level.score3Star) ? 1 : 0;

		for (int i = 0; i < stars.Length; i++)
		{
			stars[i].enabled = (i == starIdx);
		}
	}

	public void SetTarget(int target)
	{
		targetText.text = target.ToString();
	}

	public void SetRemaining(int remaining)
	{
		remainingText.text = remaining.ToString();
	}

	public void SetRemaining(string remaining)
	{
		remainingText.text = remaining;
	}

	public void SetLevelType(Level.LevelType type)
	{
		if (type == Level.LevelType.MOVES)
		{
			remainingSubtext.text = "moves remaining";
			targetSubtext.text = "target score";
		}
		else if (type == Level.LevelType.OBSTACLE)
		{
			remainingSubtext.text = "moves remaining";
			targetSubtext.text = "bubbles remaining";
		}
		else if (type == Level.LevelType.TIMER)
		{
			remainingSubtext.text = "time remaining";
			targetSubtext.text = "target score";
		}
	}

	public void OnGameWin(int score)
	{
		isGameOver = true;
	}

	public void OnGameLose()
	{
		isGameOver = true;
	}
}
