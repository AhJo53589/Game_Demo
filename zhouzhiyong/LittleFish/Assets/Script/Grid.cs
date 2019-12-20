using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grid : MonoBehaviour
{
    public enum PieceType
    {
        EMPTY,
        NORMAL,
        BUBBLE,
        ROW_CLEAR,
        COLUMN_CLEAR,
        COUNT,
    };

    [System.Serializable]
    public struct PiecePrefab
    {
        public PieceType type;
        public GameObject prefab;
    };

    public int xDim;
    public int yDim;
    public float fillTime;
    public float clearTime;

    public PiecePrefab[] piecePrefabs;
    public GameObject backgroundPrefab;

    private Dictionary<PieceType, GameObject> piecePrefabDict;
    private GamePiece[,] pieces;

    private bool inverse = false;
    private GamePiece pressedPiece;
    private GamePiece enteredPiece;

    // Use this for initialization
    void Start ()
    {
        piecePrefabDict = new Dictionary<PieceType, GameObject>();

        for (int i = 0; i < piecePrefabs.Length; i++)
        {
            if (!piecePrefabDict.ContainsKey(piecePrefabs[i].type))
            {
                piecePrefabDict.Add(piecePrefabs[i].type, piecePrefabs[i].prefab);
            }
        }

        for (int x = 0; x < xDim; x++)
        {
            for (int y = 0; y < yDim; y++)
            {
                GameObject background = (GameObject)Instantiate(backgroundPrefab, GetWorldPosition(x, y), Quaternion.identity);
                background.transform.parent = transform;
            }
        }

        pieces = new GamePiece[xDim, yDim];
        for (int x = 0; x < xDim; x++)
        {
            for (int y = 0; y < yDim; y++)
            {
                SpawnNewPiece(x, y, PieceType.EMPTY);
            }
        }

        Destroy(pieces[1, 4].gameObject);
        SpawnNewPiece(1, 4, PieceType.BUBBLE);

        Destroy(pieces[2, 4].gameObject);
        SpawnNewPiece(2, 4, PieceType.BUBBLE);

        Destroy(pieces[3, 4].gameObject);
        SpawnNewPiece(3, 4, PieceType.BUBBLE);

        Destroy(pieces[5, 4].gameObject);
        SpawnNewPiece(5, 4, PieceType.BUBBLE);

        Destroy(pieces[6, 4].gameObject);
        SpawnNewPiece(6, 4, PieceType.BUBBLE);

        Destroy(pieces[7, 4].gameObject);
        SpawnNewPiece(7, 4, PieceType.BUBBLE);

        Destroy(pieces[4, 0].gameObject);
        SpawnNewPiece(4, 0, PieceType.BUBBLE);

        StartCoroutine(Fill());
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    public IEnumerator Fill()
    {
        bool needsRefill = true;

        while (needsRefill)
        {
            yield return new WaitForSeconds(clearTime);

            while (FillStep())
            {
                inverse = !inverse;
                yield return new WaitForSeconds(fillTime);
            }

            needsRefill = ClearAllValidMatches();
        }
    }

    public bool FillStep()
    {
        bool movedPiece = false;

        for (int y = yDim - 1; y >= 0; y--)
        {
            for (int loopX = 0; loopX < xDim; loopX++)
            {
                int x = (inverse) ? xDim - loopX - 1 : loopX;
                GamePiece piece = pieces[x, y];
                if (piece.Type != PieceType.EMPTY) continue;

                if (y == 0)
                {
                    Destroy(piece.gameObject);
                    GenerateNewPieceOnFirstLine(x);
                    movedPiece = true;
                    continue;
                }

                bool hasMovableAbove = true;
                for (int tempY = y - 1; tempY >= 0; tempY--)
                {
                    GamePiece pieceTemp = pieces[x, tempY];
                    if (pieceTemp.IsMovable()) break;
                    if (!pieceTemp.IsMovable() && pieceTemp.Type != PieceType.EMPTY)
                    {
                        hasMovableAbove = false;
                        break;
                    }
                }

                int[] diagX1 = new int[] { 0, 1, -1 };
                int[] diagX2 = new int[] { 0, -1, 1 };
                int[] diagX = (inverse) ? diagX1 : diagX2;
                for (int i = 0; i < 3; i++)
                {
                    int tx = x + diagX[i];
                    int ty = y - 1;
                    if (tx < 0 || tx >= xDim) continue;
                    if (ty < 0 || ty >= yDim) continue;

                    GamePiece pieceAbove = pieces[tx, ty];
                    if (!pieceAbove.IsMovable()) continue;
                    if (pieceAbove.Type == PieceType.EMPTY) continue;
                    if (i != 0 && pieces[tx, y].Type == PieceType.EMPTY) continue;
                    if (i != 0 && hasMovableAbove) continue;

                    Destroy(piece.gameObject);
                    pieceAbove.MovableComponent.Move(x, y, fillTime);
                    pieces[x, y] = pieceAbove;
                    SpawnNewPiece(tx, ty, PieceType.EMPTY);
                    movedPiece = true;
                    break;
                }
            }
        }
        return movedPiece;
    }

    private void GenerateNewPieceOnFirstLine(int x)
    {
        GameObject newPiece = (GameObject)Instantiate(piecePrefabDict[PieceType.NORMAL], GetWorldPosition(x, -1), Quaternion.identity);
        newPiece.transform.parent = transform;

        pieces[x, 0] = newPiece.GetComponent<GamePiece>();
        pieces[x, 0].Init(x, -1, this, PieceType.NORMAL);
        pieces[x, 0].MovableComponent.Move(x, 0, fillTime);
        pieces[x, 0].ColorComponent.SetColor((ColorPiece.ColorType)Random.Range(0, pieces[x, 0].ColorComponent.NumColors));
    }

    public Vector2 GetWorldPosition(int x, int y)
    {
        return new Vector2(transform.position.x - xDim / 2.0f + x,
            transform.position.y + yDim / 2.0f - y);
    }

    public GamePiece SpawnNewPiece(int x, int y, PieceType type)
    {
        GameObject newPiece = (GameObject)Instantiate(piecePrefabDict[type], GetWorldPosition(x, y), Quaternion.identity);
        newPiece.transform.parent = transform;

        pieces[x, y] = newPiece.GetComponent<GamePiece>();
        pieces[x, y].Init(x, y, this, type);

        return pieces[x, y];
    }

    public bool IsAdjacent(GamePiece piece1, GamePiece piece2)
    {
        return (piece1.X == piece2.X && (int)Mathf.Abs(piece1.Y - piece2.Y) == 1)
            || (piece1.Y == piece2.Y && (int)Mathf.Abs(piece1.X - piece2.X) == 1);
    }

    public void SwapPieces(GamePiece piece1, GamePiece piece2)
    {
        if (piece1.IsMovable() && piece2.IsMovable())
        {
            pieces[piece1.X, piece1.Y] = piece2;
            pieces[piece2.X, piece2.Y] = piece1;

            if (GetMatch(piece1, piece2.X, piece2.Y) != null || GetMatch(piece2, piece1.X, piece1.Y) != null)
            {
                int piece1X = piece1.X;
                int piece1Y = piece1.Y;

                piece1.MovableComponent.Move(piece2.X, piece2.Y, fillTime);
                piece2.MovableComponent.Move(piece1X, piece1Y, fillTime);

                ClearAllValidMatches();

                if (piece1.Type == PieceType.ROW_CLEAR || piece1.Type == PieceType.COLUMN_CLEAR)
                {
                    ClearPiece(piece1.X, piece1.Y);
                }
                if (piece2.Type == PieceType.ROW_CLEAR || piece2.Type == PieceType.COLUMN_CLEAR)
                {
                    ClearPiece(piece2.X, piece2.Y);
                }

                pressedPiece = null;
                enteredPiece = null;

                StartCoroutine(Fill());
            }
            else
            {
                pieces[piece1.X, piece1.Y] = piece1;
                pieces[piece2.X, piece2.Y] = piece1;
            }
        }
    }

    public void PressPiece(GamePiece piece)
    {
        pressedPiece = piece;
    }

    public void EnterPiece(GamePiece piece)
    {
        enteredPiece = piece;
    }

    public void ReleasePiece()
    {
        if (IsAdjacent(pressedPiece, enteredPiece))
        {
            SwapPieces(pressedPiece, enteredPiece);
        }
    }

    private List<GamePiece> GetMatchOthersInLine(GamePiece piece, int newX, int newY, int dir)
    {
        ColorPiece.ColorType color = piece.ColorComponent.Color;
        List<GamePiece> colPieces = new List<GamePiece>();

        int[] dx = new int[] { 1, -1, 0, 0 };
        int[] dy = new int[] { 0, 0, 1, -1 };

        for (int i = dir * 2; i < 4 - (1 - dir) * 2; i++)
        {
            int tx = newX;
            int ty = newY;
            while (true)
            {
                tx += dx[i];
                ty += dy[i];
                if (tx < 0 || tx >= xDim) break;
                if (ty < 0 || ty >= yDim) break;
                if (tx == piece.X && ty == piece.Y) break;
                if (!pieces[tx, ty].IsColored() || pieces[tx, ty].ColorComponent.Color != color) break;
                colPieces.Add(pieces[tx, ty]);
            }
        }

        return colPieces;
    }

    public List<GamePiece> GetMatch(GamePiece piece, int newX, int newY)
    {
        if (!piece.IsColored()) return null;

        List<GamePiece> matchingPieces = new List<GamePiece>();

        List<GamePiece>[] colPieces = new List<GamePiece>[2];
        for (int i = 0; i < colPieces.Length; i++)
        {
            colPieces[i] = GetMatchOthersInLine(piece, newX, newY, i);
            if (colPieces[i].Count < 2) continue;

            matchingPieces.AddRange(colPieces[i]);

            foreach (var p in colPieces[i])
            {
                List<GamePiece> temp = GetMatchOthersInLine(p, p.X, p.Y, 1 - i);
                if (temp.Count < 2) continue;
                matchingPieces.AddRange(temp);
                temp.Clear();
            }
        }

        matchingPieces.Add(piece);
        if (matchingPieces.Count < 3) return null;
        return matchingPieces;
    }

    public bool ClearAllValidMatches()
    {
        bool needsRefill = false;

        for (int y = 0; y < yDim; y++)
        {
            for (int x = 0; x < xDim; x++)
            {
                if (!pieces[x, y].IsClearable()) continue;

                List<GamePiece> match = GetMatch(pieces[x, y], x, y);
                if (match == null) continue;

                PieceType specialPieceType = PieceType.COUNT;
                GamePiece randomPiece = match[Random.Range(0, match.Count)];
                int specialPieceX = randomPiece.X;
                int specialPieceY = randomPiece.Y;

                if (match.Count == 4)
                {
                    if (pressedPiece == null || enteredPiece == null)
                    {
                        specialPieceType = (PieceType)Random.Range((int)PieceType.ROW_CLEAR, (int)PieceType.COLUMN_CLEAR);
                    }
                    else if (pressedPiece.Y == enteredPiece.Y)
                    {
                        specialPieceType = PieceType.ROW_CLEAR;
                    }
                    else
                    {
                        specialPieceType = PieceType.COLUMN_CLEAR;
                    }
                }

                for (int i = 0; i < match.Count; i++)
                {
                    if (ClearPiece(match[i].X, match[i].Y))
                    {
                        needsRefill = true;

                        if (match[i] == pressedPiece || match[i] == enteredPiece)
                        {
                            specialPieceX = match[i].X;
                            specialPieceY = match[i].Y;
                        }
                    }
                }

                if (specialPieceType != PieceType.COUNT)
                {
                    Destroy(pieces[specialPieceX, specialPieceY]);
                    GamePiece newPiece = SpawnNewPiece(specialPieceX, specialPieceY, specialPieceType);

                    if ((specialPieceType == PieceType.ROW_CLEAR || specialPieceType == PieceType.COLUMN_CLEAR)
                        && newPiece.IsColored() && match[0].IsColored())
                    {
                        newPiece.ColorComponent.SetColor(match[0].ColorComponent.Color);
                    }
                }
            }
        }

        return needsRefill;
    }

    public bool ClearPiece(int x, int y)
    {
        if (pieces[x, y].IsClearable() && !pieces[x, y].ClearableComponent.IsBeingCleared)
        {
            pieces[x, y].ClearableComponent.Clear();
            SpawnNewPiece(x, y, PieceType.EMPTY);

            ClearObstacles(x, y);

            return true;
        }

        return false;
    }

    public void ClearObstacles(int x, int y)
    {
        int[] dx = new int[] { 1, -1, 0, 0 };
        int[] dy = new int[] { 0, 0, 1, -1 };

        for (int i = 0; i < 4; i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            if (tx < 0 || tx >= xDim) continue;
            if (ty < 0 || ty >= yDim) continue;
            if (pieces[tx, ty].Type != PieceType.BUBBLE) continue;
            if (!pieces[tx, ty].IsClearable()) continue;
            pieces[tx, ty].ClearableComponent.Clear();
            SpawnNewPiece(tx, ty, PieceType.EMPTY);
        }
    }

    public void ClearRow(int row)
    {
        for (int x = 0; x < xDim; x++)
        {
            ClearPiece(x, row);
        }
    }

    public void ClearColumn(int column)
    {
        for (int y = 0; y < yDim; y++)
        {
            ClearPiece(column, y);
        }
    }

}
