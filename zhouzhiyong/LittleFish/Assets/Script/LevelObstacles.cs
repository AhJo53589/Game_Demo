using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelObstacles : Level
{
	public int numMoves;
	public Grid.PieceType[] obstacleTypes;

	private int movesUsed = 0;
	private int numObstaclesLeft;

	// Use this for initialization
	void Start () 
	{
		type = LevelType.OBSTACLE;

		for (int i = 0; i < obstacleTypes.Length; i++)
		{
			numObstaclesLeft += grid.GetPiecesOfType(obstacleTypes[i]).Count;
		}
	}
	
	// Update is called once per frame
	void Update () 
	{
		
	}

	public override void OnMove()
	{
		movesUsed++;

		Debug.Log("Moves remaining: " + (numMoves - movesUsed));

		if (numMoves - movesUsed == 0 && numObstaclesLeft > 0)
		{
			GameLose();
		}
	}

	public override void OnPieceCleared(GamePiece piece)
	{
		base.OnPieceCleared(piece);

		for (int i = 0; i < obstacleTypes.Length; i++)
		{
			if (obstacleTypes[i] == piece.Type)
			{
				numObstaclesLeft--;

				if (numObstaclesLeft == 0)
				{
					currentScore += 1000 * (numMoves - movesUsed);
					Debug.Log("current score: " + currentScore);
					GameWin();
				}
			}
		}
	}
}
