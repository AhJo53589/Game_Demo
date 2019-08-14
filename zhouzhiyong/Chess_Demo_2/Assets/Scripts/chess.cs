
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class chess : MonoBehaviour {

    const int x_len = 6;
    const int y_len = 10;
    const int x_len_chessboard_next = 3;
    const int y_len_chessboard_next = 3;

    //四个锚点位置，用于计算棋子落点
    public GameObject G_LT_Pos;
    public GameObject G_RT_Pos;
    public GameObject G_LB_Pos;

    public GameObject G_P1_NextPos;
    public GameObject G_P2_NextPos;
    public GameObject G_P1_PointPos;
    public GameObject G_P2_PointPos;

    //主摄像机
    public Camera cam;
    //锚点在屏幕上的映射位置
    Vector3 V3_LT_Pos;
    Vector3 V3_RT_Pos;
    Vector3 V3_LB_Pos;
    Vector3 V3_P1_NextPos;
    Vector3 V3_P2_NextPos;
    Vector3 V3_P1_PointPos;
    Vector3 V3_P2_PointPos;
    Vector3 V3_Cur_PointPos;   //当前点选的位置

    //资源
    public Texture2D T_chess_red;   //红棋子
    public Texture2D T_chess_blue;  //蓝棋子
    public Texture2D T_chess_green; //绿棋子
    public Texture2D T_Draw;        //平局
    public Texture2D T_P1_Win;
    public Texture2D T_P2_Win;


    float m_gridWidth = 1;  //棋盘网格宽度
    float m_gridHeight = 1; //棋盘网格高度
    float m_minGridDis;     //网格宽和高中较小的一个

    Vector2[,] m_chessPos; //存储棋盘上所有可以落子的位置
    Vector2[,] m_chessPos_P1_next;
    Vector2[,] m_chessPos_P2_next; 
    int[,] m_chessState; //存储棋盘位置上的落子状态
    int[,] m_chessState_next;


    enum Game_Turn { turn_P1, turn_P2 };
    Game_Turn m_chessTurn; //落子顺序

    int m_winner = 0; //获胜方，1为P1，2为P2
    bool m_isStart = false;
    bool m_isPlaying = true; //是否处于对弈状态
    bool m_isPlayingAnimation = false;

    Vector2Int m_chessPos_current;

    ColoredChess m_nextChess;
    int[] m_playerScore;


    void Start()
    {
        m_chessPos = new Vector2[x_len, y_len];
        m_chessPos_P1_next = new Vector2[x_len_chessboard_next, y_len_chessboard_next];
        m_chessPos_P2_next = new Vector2[x_len_chessboard_next, y_len_chessboard_next];
        m_chessState = new int[x_len, y_len];
        m_chessState_next = new int[x_len_chessboard_next, y_len_chessboard_next];
        m_chessTurn = Game_Turn.turn_P1;
        m_playerScore = new int[2];
        m_playerScore[(int)Game_Turn.turn_P1] = 0;
        m_playerScore[(int)Game_Turn.turn_P2] = 0;
    }

    void Update()
    {
        InitPos();

        if (m_isPlayingAnimation) return;


        Input_Mouse();
        Input_Restart();
        if (m_isPlaying && !m_isStart)
        {
            m_isStart = true;
            OnNextTurn();
        }
    }

    void InitPos()
    {
        //计算锚点位置
        V3_LT_Pos = cam.WorldToScreenPoint(G_LT_Pos.transform.position);
        V3_RT_Pos = cam.WorldToScreenPoint(G_RT_Pos.transform.position);
        V3_LB_Pos = cam.WorldToScreenPoint(G_LB_Pos.transform.position);
        V3_P1_NextPos = cam.WorldToScreenPoint(G_P1_NextPos.transform.position);
        V3_P2_NextPos = cam.WorldToScreenPoint(G_P2_NextPos.transform.position);
        V3_P1_PointPos = cam.WorldToScreenPoint(G_P1_PointPos.transform.position);
        V3_P2_PointPos = cam.WorldToScreenPoint(G_P2_PointPos.transform.position);

        //计算网格宽度
        m_gridWidth = (V3_RT_Pos.x - V3_LT_Pos.x) / (x_len - 1);
        m_gridHeight = (V3_LT_Pos.y - V3_LB_Pos.y) / (y_len - 1);
        m_minGridDis = m_gridWidth < m_gridHeight ? m_gridWidth : m_gridHeight;

        //计算落子点位置
        for (int i = 0; i < x_len; i++)
        {
            for (int j = 0; j < y_len; j++)
            {
                m_chessPos[i, j] = new Vector2(V3_LB_Pos.x + m_gridWidth * i, V3_LB_Pos.y + m_gridHeight * j);
            }
        }
        for (int i = 0; i < x_len_chessboard_next; i++)
        {
            for (int j = 0; j < y_len_chessboard_next; j++)
            {
                m_chessPos_P1_next[i, j] = new Vector2(V3_P1_NextPos.x + m_gridWidth * i, V3_P1_NextPos.y + m_gridHeight * j);
                m_chessPos_P2_next[i, j] = new Vector2(V3_P2_NextPos.x + m_gridWidth * i, V3_P2_NextPos.y + m_gridHeight * j);
            }
        }
    }

    void Input_Mouse()
    {
        //检测鼠标输入并确定落子状态
        if (m_isPlaying)
        {
            V3_Cur_PointPos = Input.mousePosition;
            for (int i = 0; i < x_len; i++)
            {
                for (int j = 0; j < y_len; j++)
                {
                    //找到最接近鼠标点击位置的落子点，如果空则落子
                    if (Dis(V3_Cur_PointPos, m_chessPos[i, j]) < m_minGridDis / 2 && m_chessState[i, j] == 0)
                    {
                        m_chessPos_current = new Vector2Int(i, j);
                        if (Input.GetMouseButtonUp(0))
                        {
                            if (!CheckPlacedEnable(m_chessPos_current, ref m_nextChess, ref m_chessState)) break;
                            PlacedChess(m_chessPos_current, ref m_nextChess, ref m_chessState);

                            m_isPlayingAnimation = true;
                            Invoke("OnPlacedChess", 1f);
                            m_chessPos_current = new Vector2Int(-1, -1);
                        }
                    }
                }
            }
        }
    }

    void Input_Restart()
    {
        //按下空格重新开始游戏
        if (Input.GetKeyDown(KeyCode.Space))
        {
            for (int i = 0; i < x_len; i++)
            {
                for (int j = 0; j < y_len; j++)
                {
                    m_chessState[i, j] = 0;
                }
            }
            m_isStart = false;
            m_isPlaying = true;
            m_chessTurn = Game_Turn.turn_P1;
            m_winner = 0;
        }
    }

    void OnPlacedChess()
    {
        m_isPlayingAnimation = false;

        MatchAndRemove();
        CalcPlayerScore();

        if (CheckGameOver())
        {
            OnGameOver();
            return;
        }

        OnNextTurn();
    }

    void MatchAndRemove()
    {
        for (int x = 0; x < x_len; x++)
        {
            for (int y = 0; y < y_len; y++)
            {
                if (m_chessState[x, y] != 0)
                {
                    List<Vector2> checkList = new List<Vector2>();
                    HashSet<Vector2> visitedList = new HashSet<Vector2>();
                    CheckMatch(x, y, m_chessState[x, y], ref checkList, ref visitedList);
                    if (checkList.Count > 2)
                    {
                        checkList.ForEach(v2 => { m_chessState[(int)v2.x, (int)v2.y] = 0; });
                    }
                }
            }
        }
    }

    void CalcPlayerScore()
    {
        m_playerScore[(int)Game_Turn.turn_P1] = 0;
        m_playerScore[(int)Game_Turn.turn_P2] = 0;
        for (int x = 0; x < x_len; x++)
        {
            for (int y = 0; y < y_len; y++)
            {
                if (m_chessState[x, y] != 0)
                {
                    int i = y < y_len / 2 ? 1 : 0;
                    m_playerScore[i]++;
                }
            }
        }
    }

    void OnNextTurn()
    {
        //落子成功，更换下棋顺序
        m_chessTurn = (m_chessTurn == Game_Turn.turn_P1) ? Game_Turn.turn_P2 : Game_Turn.turn_P1;

        m_nextChess = new ColoredChess();
        m_nextChess.NewRandomChess(new Vector2Int(0,0));

        for (int x = 0; x < x_len_chessboard_next; x++)
        {
            for (int y = 0; y < y_len_chessboard_next; y++)
            {
                m_chessState_next[x, y] = 0;
            }
        }
        PlacedChess(new Vector2Int(1,1), ref m_nextChess, ref m_chessState_next);
    }

    void OnGameOver()
    {
        m_isPlaying = false;
        if (m_playerScore[(int)Game_Turn.turn_P1] > m_playerScore[(int)Game_Turn.turn_P2]) m_winner = 1;
        else if (m_playerScore[(int)Game_Turn.turn_P1] < m_playerScore[(int)Game_Turn.turn_P2]) m_winner = 2;
        else m_winner = 0;
    }

    void CheckMatch(int x, int y, int k, ref List<Vector2> checkList, ref HashSet<Vector2> visitedList)
    {
        if (x >= x_len || x < 0) return;
        if (y >= y_len || y < 0) return;

        if (visitedList.Contains(new Vector2(x, y))) return;
        visitedList.Add(new Vector2(x, y));

        if (m_chessState[x, y] != k) return;
        checkList.Add(new Vector2(x, y));

        if (x + 1 < x_len) CheckMatch(x + 1, y, k, ref checkList, ref visitedList);
        if (x - 1 >= 0) CheckMatch(x - 1, y, k, ref checkList, ref visitedList);
        if (y + 1 < y_len) CheckMatch(x, y + 1, k, ref checkList, ref visitedList);
        if (y - 1 >= 0) CheckMatch(x, y - 1, k, ref checkList, ref visitedList);
    }

    bool CheckPlacedEnable(Vector2Int pos, ref ColoredChess cType, ref int[,] chessState)
    {
        ColoredChess next = new ColoredChess();
        next.InitChess(pos, ref cType);
        foreach (Vector2Int p in next.m_pos)
        {
            if (p.x >= x_len || p.x < 0) return false;
            if (p.y >= y_len || p.y < 0) return false;
            if (chessState[p.x, p.y] != 0) return false;
        }
        return true;
    }

    void PlacedChess(Vector2Int pos, ref ColoredChess cType, ref int[,] chessboard)
    {
        ColoredChess next = new ColoredChess();
        next.InitChess(pos, ref cType);
        foreach (KeyValuePair<Vector2Int, int> p in next.GetChessPosAndColor())
        {
            chessboard[p.Key.x, p.Key.y] = p.Value;
        }
    }

    bool CheckGameOver()
    {
        for (int x = 0; x < x_len; x++)
        {
            for (int y = 0; y < y_len; y++)
            {
                if (m_chessState[x, y] != 0) continue;
                if (CheckPlacedEnable(new Vector2Int(x,y), ref m_nextChess, ref m_chessState)) return false;
            }
        }
        return true;
    }


    //计算平面距离函数
    float Dis(Vector3 mPos, Vector2 gridPos)
    {
        return Mathf.Sqrt(Mathf.Pow(mPos.x - gridPos.x, 2) + Mathf.Pow(mPos.y - gridPos.y, 2));
    }

    Texture2D GetChessColorTexture(int val)
    {
        if (val == 1) return T_chess_red;
        else if (val == 2) return T_chess_blue;
        else if (val == 3) return T_chess_green;
        return T_chess_red;
    }

    void OnGUI()
    { 
        //绘制棋子
        for(int i=0;i< x_len; i++)
        {
            for (int j = 0; j < y_len; j++)
            {
                Texture2D tempT = GetChessColorTexture(m_chessState[i, j]);
                if (m_chessState[i, j] != 0)
                {
                    GUI.DrawTexture(new Rect(m_chessPos[i, j].x - m_gridWidth / 2, Screen.height - m_chessPos[i, j].y - m_gridHeight / 2, m_gridWidth, m_gridHeight), tempT);
                }
            }
        }
        //绘制下一步棋子
        for (int i = 0; i < x_len_chessboard_next; i++)
        {
            for (int j = 0; j < y_len_chessboard_next; j++)
            {
                Texture2D tempT = GetChessColorTexture(m_chessState_next[i, j]);

                Vector2[,] tempV = m_chessPos_P1_next;
                if (m_chessTurn == Game_Turn.turn_P1) tempV = m_chessPos_P1_next;
                else if (m_chessTurn == Game_Turn.turn_P2) tempV = m_chessPos_P2_next;

                if (m_chessState_next[i, j] != 0)
                {
                    GUI.DrawTexture(new Rect(tempV[i, j].x - m_gridWidth / 2, Screen.height - tempV[i, j].y - m_gridHeight / 2, m_gridWidth, m_gridHeight), tempT);
                }
            }
        }
        //绘制鼠标上的棋子
        if (CheckPlacedEnable(m_chessPos_current, ref m_nextChess, ref m_chessState))
        {
            ColoredChess next = new ColoredChess();
            next.InitChess(m_chessPos_current, ref m_nextChess);
            foreach (KeyValuePair<Vector2Int, int> p in next.GetChessPosAndColor())
            {
                int i = p.Key.x;
                int j = p.Key.y;

                Texture2D tempT = GetChessColorTexture(p.Value);

                GUI.DrawTexture(new Rect(m_chessPos[i, j].x - m_gridWidth / 2, Screen.height - m_chessPos[i, j].y - m_gridHeight / 2, m_gridWidth, m_gridHeight), tempT);
            }
        }

        // 分数
        //GUI.DrawTexture(new Rect(V3_P1_PointPos.x - m_gridWidth / 2, Screen.height - V3_P1_PointPos.y - m_gridHeight / 2, m_gridWidth, m_gridHeight), T_chess_red);
        //GUI.DrawTexture(new Rect(V3_P2_PointPos.x - m_gridWidth / 2, Screen.height - V3_P2_PointPos.y - m_gridHeight / 2, m_gridWidth, m_gridHeight), T_chess_red);

        GUIStyle style = new GUIStyle();
        style.normal.background = null;    //这是设置背景填充的
        style.normal.textColor = new Color(230, 0, 0);   //字体颜色
        style.fontSize = 72;       //字体大小

        string score = m_playerScore[(int)Game_Turn.turn_P1].ToString();
        GUI.Label(new Rect(V3_P1_PointPos.x + m_gridWidth, Screen.height - V3_P1_PointPos.y - m_gridHeight / 2, m_gridWidth, m_gridHeight), score, style);
        score = m_playerScore[(int)Game_Turn.turn_P2].ToString();
        GUI.Label(new Rect(V3_P2_PointPos.x + m_gridWidth, Screen.height - V3_P2_PointPos.y - m_gridHeight / 2, m_gridWidth, m_gridHeight), score, style);

        //根据获胜状态，弹出相应的胜利图片
        if (!m_isPlaying)
        {
            if (m_winner == 0)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), T_Draw);
            if (m_winner == 1)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), T_P1_Win);
            if (m_winner == 2)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), T_P2_Win);
        }
    }
    }


