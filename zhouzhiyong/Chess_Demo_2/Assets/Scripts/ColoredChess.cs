using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ColoredChess
{
    public enum ChessShapeType
    {
        CST_Grid2 = 0,
        CST_Grid3_L,
        CST_Grid3_I,
        CST_Grid4_T,
        CST_Grid5_Plus
    }

    public ColoredChess();

    public void NewRandomChess(Vector2Int pos)
    {
        ChessShapeType shape = Random.Range(0, 4);
        int direction = Random.Range(0, 4);
        List<int> color;                  
        for (int i = 0; i < 5; i++) color.Add(Random.Range(0, 3));
        InitChess(shape, direction, color, pos);
    }

    public void InitChess(ChessShapeType shape, int direction, List<int>color, Vector2Int pos)
    {
        m_shape = shape;
        m_direction = direction;
        m_color = color;
        m_pos.Clear();
        m_pos.Add(pos);
        List<Vector2Int> temp = CalcAttachChessOffset(CalcAttachChessId(shape, direction), pos);
        m_pos.AddRange(temp);
    }

    private List<int> CalcAttachChessId(ChessShapeType shape, int direction)
    {
        List<int> attachChess;
        switch (shape)
        {
            case ChessShapeType.CST_Grid2:
                attachChess.Add((0 + direction) % 4);
                break;
            case ChessShapeType.CST_Grid3_L:
                attachChess.Add((0 + direction) % 4);
                attachChess.Add((1 + direction) % 4);
                break;
            case ChessShapeType.CST_Grid3_I:
                attachChess.Add((0 + direction) % 4);
                attachChess.Add((2 + direction) % 4);
                break;
            case ChessShapeType.CST_Grid4_T:
                attachChess.Add((0 + direction) % 4);
                attachChess.Add((1 + direction) % 4);
                attachChess.Add((2 + direction) % 4);
                break;
            case ChessShapeType.CST_Grid5_Plus:
                attachChess.Add((0 + direction) % 4);
                attachChess.Add((1 + direction) % 4);
                attachChess.Add((2 + direction) % 4);
                attachChess.Add((3 + direction) % 4);
                break;
            default:
        }
        return attachChess; 
    }

    private List<Vector2Int> CalcAttachChessOffset(List<int> attachChess, Vector2Int pos)
    {
        List<Vector2Int> attachPos;
        foreach (var i in attachChess)
        {
            if (i == 0) attachPos.Add(new Vector2Int(pos.x, pos.y - 1));
            else if (i == 1) attachPos.Add(new Vector2Int(pos.x + 1, pos.y));
            else if (i == 2) attachPos.Add(new Vector2Int(pos.x, pos.y + 1));
            else if (i == 3) attachPos.Add(new Vector2Int(pos.x - 1, pos.y));
        }
        return attachPos;
    }

    public ChessShapeType m_shape
    {
        get;
        set;
    }
    public int m_direction
    {
        get;
        set;
    }
    public List<int> m_color
    {
        get;
        set;
    }
    public List<Vector2Int> m_pos
    {
        get;
        set;
    }
}
