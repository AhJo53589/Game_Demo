using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelTimer : Level
{
	public int timeInSeconds;
	public int targetScore;

	private float timer;
	private bool timeOut = false;

	// Use this for initialization
	void Start () 
	{
		type = LevelType.TIMER;

		hud.SetLevelType(type);
		hud.SetScore(currentScore);
		hud.SetTarget(targetScore);
		hud.SetRemaining(string.Format("{0}:{1:00}", timeInSeconds / 60, timeInSeconds % 60));
	}

	// Update is called once per frame
	void Update () 
	{
		if (timeOut) return;

		timer += Time.deltaTime;
		float remainingTime = timeInSeconds - timer;
		hud.SetRemaining(string.Format("{0}:{1:00}", (int)Mathf.Max(remainingTime / 60, 0), (int)Mathf.Max(remainingTime % 60, 0)));

		if (remainingTime <= 0)
		{
			if (currentScore >= targetScore)
			{
				GameWin();
			}
			else
			{
				GameLose();
			}
		}
	}
}
