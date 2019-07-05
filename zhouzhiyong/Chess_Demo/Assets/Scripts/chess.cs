
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class chess : MonoBehaviour {

    const int x_len = 6;
    const int y_len = 10;
    const int x_len_chessboard_next = 3;
    const int y_len_chessboard_next = 3;

    //四个锚点位置，用于计算棋子落点
    public GameObject LeftTop;
    public GameObject RightTop;
    public GameObject LeftBottom;
    public GameObject RightBottom;
    public GameObject RedNext;
    public GameObject BlueNext;
    public GameObject RedPoint;
    public GameObject BluePoint;

    //主摄像机
    public Camera cam;
    //锚点在屏幕上的映射位置
    Vector3 LTPos;
    Vector3 RTPos;
    Vector3 LBPos;
    Vector3 RedNextPos;
    Vector3 BlueNextPos;
    Vector3 RedPointPos;
    Vector3 BluePointPos;

    Vector3 PointPos;//当前点选的位置
    float gridWidth = 1; //棋盘网格宽度
    float gridHeight = 1; //棋盘网格高度
    float minGridDis;  //网格宽和高中较小的一个
    Vector2[,] chessPos; //存储棋盘上所有可以落子的位置
    Vector2[,] chessPos_nextRed;
    Vector2[,] chessPos_nextBlue;
    int[,] chessState; //存储棋盘位置上的落子状态
    int[,] chessState_next;
    enum turn { red, blue };
    turn chessTurn; //落子顺序
    public Texture2D white; //白棋子
    public Texture2D black; //黑棋子
    public Texture2D draw; //平局
    public Texture2D blackWin; //白子获胜提示图
    public Texture2D whiteWin; //黑子获胜提示图
    int winner = 0; //获胜方，1为黑子，-1为白子
    bool isStart = false;
    bool isPlaying = true; //是否处于对弈状态
    bool isPlayingAnimation = false;

    ChessType nextChessType;
    int[] playerScore;


    enum ChessType { typeA1, typeB1, typeC1, typeA2, typeB2, typeC2 };

    void Start() {
        chessPos = new Vector2[x_len, y_len];
        chessPos_nextRed = new Vector2[x_len_chessboard_next, y_len_chessboard_next];
        chessPos_nextBlue = new Vector2[x_len_chessboard_next, y_len_chessboard_next];
        chessState = new int[x_len, y_len];
        chessState_next = new int[x_len_chessboard_next, y_len_chessboard_next];
        chessTurn = turn.red;
        playerScore = new int[2];
        playerScore[(int)turn.red] = 0;
        playerScore[(int)turn.blue] = 0;
    }

    void Update() {

        if (isPlayingAnimation) return;

        //计算锚点位置
        LTPos = cam.WorldToScreenPoint(LeftTop.transform.position);
        RTPos = cam.WorldToScreenPoint(RightTop.transform.position);
        LBPos = cam.WorldToScreenPoint(LeftBottom.transform.position);
        RedNextPos = cam.WorldToScreenPoint(RedNext.transform.position);
        BlueNextPos = cam.WorldToScreenPoint(BlueNext.transform.position);
        RedPointPos = cam.WorldToScreenPoint(RedPoint.transform.position);
        BluePointPos = cam.WorldToScreenPoint(BluePoint.transform.position);

        //计算网格宽度
        gridWidth = (RTPos.x - LTPos.x) / (x_len - 1);
        gridHeight = (LTPos.y - LBPos.y) / (y_len - 1);
        minGridDis = gridWidth < gridHeight ? gridWidth : gridHeight;

        //计算落子点位置
        for (int i = 0; i < x_len; i++)
        {
            for (int j = 0; j < y_len; j++)
            {
                chessPos[i, j] = new Vector2(LBPos.x + gridWidth * i, LBPos.y + gridHeight * j);
            }
        }
        for (int i = 0; i < x_len_chessboard_next; i++)
        {
            for (int j = 0; j < y_len_chessboard_next; j++)
            {
                chessPos_nextRed[i, j] = new Vector2(RedNextPos.x + gridWidth * i, RedNextPos.y + gridHeight * j);
                chessPos_nextBlue[i, j] = new Vector2(BlueNextPos.x + gridWidth * i, BlueNextPos.y + gridHeight * j);
            }
        }

        //检测鼠标输入并确定落子状态
        if (isPlaying && Input.GetMouseButtonDown(0))
        {
            PointPos = Input.mousePosition;
            for (int i = 0; i < x_len; i++)
            {
                for (int j = 0; j < y_len; j++)
                {
                    //找到最接近鼠标点击位置的落子点，如果空则落子
                    if (Dis(PointPos, chessPos[i, j]) < minGridDis / 2 && chessState[i, j] == 0)
                    {
                        if (!CheckPlacedEnable(i, j, nextChessType)) break;
                        PlacedChess(i, j, nextChessType, ref chessState);

                        isPlayingAnimation = true;
                        Invoke("OnPlacedChess", 1f);
                    }
                }
            }
            //调用判断函数，确定是否有获胜方
            //int re = result();
            //if (re == 1)
            //{
            //    Debug.Log("黑棋胜");
            //    winner = 1;
            //    isPlaying = false;
            //}
            //else if(re==-1)
            //{
            //    Debug.Log("白棋胜");
            //    winner = -1;
            //    isPlaying = false;
            //}            
        }
        //按下空格重新开始游戏
        if (Input.GetKeyDown(KeyCode.Space))
        {
            for (int i = 0; i < x_len; i++)
            {
                for (int j = 0; j < y_len; j++)
                {
                    chessState[i, j] = 0;
                }
            }
            isStart = false;
            isPlaying = true;
            chessTurn = turn.red;
            winner = 0;
        }
        if (isPlaying && !isStart)
        {
            isStart = true;
            OnNextTurn();
        }
    }

    void OnPlacedChess()
    {
        isPlayingAnimation = false;

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
                if (chessState[x, y] != 0)
                {
                    List<Vector2> checkList = new List<Vector2>();
                    HashSet<Vector2> visitedList = new HashSet<Vector2>();
                    CheckMatch(x, y, chessState[x, y], ref checkList, ref visitedList);
                    if (checkList.Count > 2)
                    {
                        checkList.ForEach(v2 => { chessState[(int)v2.x, (int)v2.y] = 0; });
                    }
                }
            }
        }
    }

    void CalcPlayerScore()
    {
        playerScore[(int)turn.red] = 0;
        playerScore[(int)turn.blue] = 0;
        for (int x = 0; x < x_len; x++)
        {
            for (int y = 0; y < y_len; y++)
            {
                if (chessState[x, y] == -1)
                {
                    int i = y < y_len / 2 ? 1 : 0;
                    playerScore[i]++;
                }
            }
        }
    }

    void OnNextTurn()
    {
        //落子成功，更换下棋顺序
        chessTurn = (chessTurn == turn.red) ? turn.blue : turn.red;

        int iChessType = Random.Range(0, 3);
        nextChessType = (chessTurn == turn.red) ? (ChessType)iChessType : (ChessType)iChessType + 3;
        Debug.Log("iChessType = " + nextChessType.ToString());

        for (int x = 0; x < x_len_chessboard_next; x++)
        {
            for (int y = 0; y < y_len_chessboard_next; y++)
            {
                chessState_next[x, y] = 0;
            }
        }
        PlacedChess(1, 1, nextChessType, ref chessState_next);
    }

    void OnGameOver()
    {
        isPlaying = false;
        if (playerScore[(int)turn.red] > playerScore[(int)turn.blue]) winner = 1;
        else if (playerScore[(int)turn.red] < playerScore[(int)turn.blue]) winner = -1;
        else winner = 0;
    }

    void CheckMatch(int x, int y, int k, ref List<Vector2> checkList, ref HashSet<Vector2> visitedList)
    {
        if (x >= x_len || x < 0) return;
        if (y >= y_len || y < 0) return;

        if (visitedList.Contains(new Vector2(x, y))) return;
        visitedList.Add(new Vector2(x, y));

        if (chessState[x, y] != k) return;
        checkList.Add(new Vector2(x, y));

        if (x + 1 < x_len) CheckMatch(x + 1, y, k, ref checkList, ref visitedList);
        if (x - 1 >= 0) CheckMatch(x - 1, y, k, ref checkList, ref visitedList);
        if (y + 1 < y_len) CheckMatch(x, y + 1, k, ref checkList, ref visitedList);
        if (y - 1 >= 0) CheckMatch(x, y - 1, k, ref checkList, ref visitedList);
    }

    bool CheckPlacedEnable(int x, int y, ChessType cType)
    {
        if (x >= x_len || x < 0) return false;
        if (y >= y_len || y < 0) return false;
        if (chessState[x, y] != 0) return false;

        bool bUp = !(y + 1 >= y_len || y + 1 < 0);
        bool bDown = !(y - 1 >= y_len || y - 1 < 0);
        bool bLeft = !(x - 1 >= x_len || x - 1 < 0);
        bool bRight = !(x + 1 >= x_len || x + 1 < 0);

        if (cType == ChessType.typeA1 && bDown) return (chessState[x, y - 1] == 0);
        if (cType == ChessType.typeA2 && bUp) return (chessState[x, y + 1] == 0);
        if (cType == ChessType.typeB1 && bDown && bLeft) return (chessState[x, y - 1] == 0) && (chessState[x - 1, y] == 0);
        if (cType == ChessType.typeB2 && bUp && bLeft) return (chessState[x, y + 1] == 0) && (chessState[x - 1, y] == 0);
        if (cType == ChessType.typeC1 && bDown && bLeft && bRight) return (chessState[x, y - 1] == 0) && (chessState[x - 1, y] == 0) && (chessState[x + 1, y] == 0);
        if (cType == ChessType.typeC2 && bUp && bLeft && bRight) return (chessState[x, y + 1] == 0) && (chessState[x - 1, y] == 0) && (chessState[x + 1, y] == 0);
        return false;
    }

    void PlacedChess(int x, int y, ChessType cType, ref int[,] chessboard)
    {
        switch (cType)
        {
            case ChessType.typeA1:
                chessboard[x, y] = 1;
                chessboard[x, y - 1] = -1;
                break;
            case ChessType.typeB1:
                chessboard[x, y] = 1;
                chessboard[x, y - 1] = -1;
                chessboard[x - 1, y] = -1;
                break;
            case ChessType.typeC1:
                chessboard[x, y] = 1;
                chessboard[x, y - 1] = -1;
                chessboard[x - 1, y] = -1;
                chessboard[x + 1, y] = -1;
                break;
            case ChessType.typeA2:
                chessboard[x, y] = 1;
                chessboard[x, y + 1] = -1;
                break;
            case ChessType.typeB2:
                chessboard[x, y] = 1;
                chessboard[x, y + 1] = -1;
                chessboard[x - 1, y] = -1;
                break;
            case ChessType.typeC2:
                chessboard[x, y] = 1;
                chessboard[x, y + 1] = -1;
                chessboard[x + 1, y] = -1;
                chessboard[x - 1, y] = -1;
                break;
        }
    }

    bool CheckGameOver()
    {
        for (int x = 0; x < x_len; x++)
        {
            for (int y = 0; y < y_len; y++)
            {
                if (chessState[x, y] != 0) continue;
                if (CheckPlacedEnable(x, y, nextChessType)) return false;
            }
        }
        return true;
    }


    //计算平面距离函数
    float Dis(Vector3 mPos, Vector2 gridPos)
    {
        return Mathf.Sqrt(Mathf.Pow(mPos.x - gridPos.x, 2) + Mathf.Pow(mPos.y - gridPos.y, 2));
    }

    void OnGUI()
    { 
        //绘制棋子
        for(int i=0;i< x_len; i++)
        {
            for (int j = 0; j < y_len; j++)
            {
                if (chessState[i, j] == 1)
                {
                    GUI.DrawTexture(new Rect(chessPos[i,j].x-gridWidth/2, Screen.height-chessPos[i,j].y-gridHeight/2, gridWidth,gridHeight),black);
                }
                if (chessState[i, j] == -1)
                {
                    GUI.DrawTexture(new Rect(chessPos[i, j].x - gridWidth / 2, Screen.height - chessPos[i, j].y - gridHeight / 2, gridWidth, gridHeight), white);
                }               
            }
        }
        for (int i = 0; i < x_len_chessboard_next; i++)
        {
            for (int j = 0; j < y_len_chessboard_next; j++)
            {
                if (chessState_next[i, j] == 1)
                {
                    if (chessTurn == turn.red)
                    {
                        GUI.DrawTexture(new Rect(chessPos_nextRed[i, j].x - gridWidth / 2, Screen.height - chessPos_nextRed[i, j].y - gridHeight / 2, gridWidth, gridHeight), black);
                    }
                    else
                    {
                        GUI.DrawTexture(new Rect(chessPos_nextBlue[i, j].x - gridWidth / 2, Screen.height - chessPos_nextBlue[i, j].y - gridHeight / 2, gridWidth, gridHeight), black);
                    }
                }
                if (chessState_next[i, j] == -1)
                {
                    if (chessTurn == turn.red)
                    {
                        GUI.DrawTexture(new Rect(chessPos_nextRed[i, j].x - gridWidth / 2, Screen.height - chessPos_nextRed[i, j].y - gridHeight / 2, gridWidth, gridHeight), white);
                    }
                    else
                    {
                        GUI.DrawTexture(new Rect(chessPos_nextBlue[i, j].x - gridWidth / 2, Screen.height - chessPos_nextBlue[i, j].y - gridHeight / 2, gridWidth, gridHeight), white);
                    }
                }
            }
        }

        // 分数
        GUI.DrawTexture(new Rect(RedPointPos.x - gridWidth / 2, Screen.height - RedPointPos.y - gridHeight / 2, gridWidth, gridHeight), white);
        GUI.DrawTexture(new Rect(BluePointPos.x - gridWidth / 2, Screen.height - BluePointPos.y - gridHeight / 2, gridWidth, gridHeight), white);

        GUIStyle style = new GUIStyle();
        style.normal.background = null;    //这是设置背景填充的
        style.normal.textColor = new Color(230, 0, 0);   //字体颜色
        style.fontSize = 72;       //字体大小

        string score = playerScore[(int)turn.red].ToString();
        GUI.Label(new Rect(RedPointPos.x + gridWidth, Screen.height - RedPointPos.y - gridHeight / 2, gridWidth, gridHeight), score, style);
        score = playerScore[(int)turn.blue].ToString();
        GUI.Label(new Rect(BluePointPos.x + gridWidth, Screen.height - BluePointPos.y - gridHeight / 2, gridWidth, gridHeight), score, style);

        //根据获胜状态，弹出相应的胜利图片
        if (!isPlaying)
        {
            if (winner == 0)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), draw);
            if (winner == 1)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), blackWin);
            if (winner == -1)
                GUI.DrawTexture(new Rect(Screen.width * 0.3f, Screen.height * 0.45f, Screen.width * 0.3f, Screen.height * 0.15f), whiteWin);
        }
    }
    }


