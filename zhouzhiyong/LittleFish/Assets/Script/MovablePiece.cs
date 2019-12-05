using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovablePiece : MonoBehaviour
{
    private GamePiece piece;

    void Awake()
    {
        piece = GetComponent<GamePiece>();
    }

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public void Move(int newX, int newY)
    {
        piece.X = newX;
        piece.Y = newY;

        piece.transform.localPosition = piece.GridRef.GetWorldPosition(newX, newY);
    }
}
