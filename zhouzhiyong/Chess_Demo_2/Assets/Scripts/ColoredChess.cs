using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ColoredChess
{
    public enum ChessShapeType
    {
        CST_Grid_None = 0,
        CST_Grid2,
        CST_Grid3_L,
        CST_Grid3_I,
        CST_Grid4_T,
        CST_Grid5_Plus
    }

    public ColoredChess() { }

    public void NewRandomChess(Vector2Int pos)
    {
        ChessShapeType shape = (ChessShapeType)Random.Range(1, 6);
        int direction = Random.Range(0, 4);
        List<int> color = GetChessColor();
        InitChess(pos, shape, direction, color);
    }

    //public void InitChess(List<Vector2Int> pos, List<int> color)
    //{
    //    m_shape = shape;
    //    m_direction = direction;
    //    m_color = color;
    //    m_pos.Clear();
    //    m_pos.Add(pos);
    //    List<Vector2Int> temp = CalcAttachChessOffset(CalcAttachChessId(shape, direction), pos);
    //    m_pos.AddRange(temp);
    //}

    public void InitChess(Vector2Int pos, ref ColoredChess chess)
    {
        m_shape = chess.m_shape;
        m_direction = chess.m_direction;
        m_color = new List<int>(chess.m_color);
        m_pos = new List<Vector2Int>();
        m_pos.Add(pos);
        List<Vector2Int> temp = CalcAttachChessOffset(CalcAttachChessId(m_shape, m_direction), pos);
        m_pos.AddRange(temp);
    }

    public void InitChess(Vector2Int pos, ChessShapeType shape, int direction, List<int>color)
    {
        m_shape = shape;
        m_direction = direction;
        m_color = new List<int>(color);
        m_pos = new List<Vector2Int>();
        m_pos.Add(pos);
        List<Vector2Int> temp = CalcAttachChessOffset(CalcAttachChessId(shape, direction), pos);
        m_pos.AddRange(temp);
    }

    public IEnumerable<KeyValuePair<Vector2Int, int>> GetChessPosAndColor()
    {
        for (int i = 0; i < m_pos.Count; i++)
        {
            KeyValuePair<Vector2Int, int> kvp = new KeyValuePair<Vector2Int, int>(m_pos[i], m_color[i]);
            yield return kvp;
        }
        yield break;
    }

    private List<int> GetChessColor()
    {
        List<int> color = new List<int>();
        color.Add(1);
        for (int i = 1; i < 5; i++)
        {
            color.Add(Random.Range(2, 4));
        }
        int temp = Random.Range(1, 4);
        for (int i = 0; i < 5; i++)
        {
            color[i] = (color[i] + temp) % 3 + 1;
        }
        return color;
    }

    private List<int> CalcAttachChessId(ChessShapeType shape, int direction)
    {
        List<int> attachChess = new List<int>();
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
                break;
        }
        return attachChess; 
    }

    private List<Vector2Int> CalcAttachChessOffset(List<int> attachChess, Vector2Int pos)
    {
        List<Vector2Int> attachPos = new List<Vector2Int>();
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
