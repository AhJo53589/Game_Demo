using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChessBoard
{
    public enum ChessShapeType
    {
        CST_Grid2 = 0,
        CST_Grid3_L,
        CST_Grid3_I,
        CST_Grid4_T,
        CST_Grid5_Plus
    }

    public ChessBoard();

    public Vector2[,] chessPos //存储棋盘上所有可以落子的位置
    {
        get;
        set;
    }
    public int[,] chessState //存储棋盘位置上的落子状态
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
