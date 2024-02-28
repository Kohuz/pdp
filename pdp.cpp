#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>
#include <cmath>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

struct Rectangle
{
    int x1, y1, x2, y2;
};

struct Point
{
    int x;
    int y;
    int depth;
};

struct Board
{
    int m; // Number of rows
    int n; // Number of columns
    vector<char> boardState;
    Rectangle white;
    Rectangle black;
};
int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

int recursionCounter = 0;

Board createBoard(int m, int n, const Rectangle &white, const Rectangle &black)
{
    vector<char> boardState(m * n, '-');
    for (int i = white.x1; i <= white.x2; ++i)
    {
        for (int j = white.y1; j <= white.y2; ++j)
        {
            boardState[i * n + j] = 'W';
        }
    }

    for (int i = black.x1; i <= black.x2; ++i)
    {
        for (int j = black.y1; j <= black.y2; ++j)
        {
            boardState[i * n + j] = 'B';
        }
    }
    Board board;
    board.boardState = boardState;
    board.m = m;
    board.n = n;
    board.white = white;
    board.black = black;
    return board;
}

void printBoard(Board board)
{
    for (int i = 0; i < board.m; ++i)
    {
        for (int j = 0; j < board.n; ++j)
        {
            cout << board.boardState[i * board.n + j] << ' ';
        }
        cout << endl;
    }
}

bool isInBoard(int x, int y, Board board)
{
    return x >= 0 && x < board.m && y >= 0 && y < board.n;
}

bool isInTargetArea(int x, int y, Rectangle targetArea)
{
    return x >= targetArea.x1 && x <= targetArea.x2 && y >= targetArea.y1 && y <= targetArea.y2;
}

int bfs(const Board &board, int startX, int startY, Rectangle targetArea)
{
    // Create a visited array to mark visited cells using 1D array
    vector<bool> visited(board.m * board.n, false);
    // Create a queue for BFS
    queue<Point> q;
    // Mark the starting point as visited and enqueue it
    visited[startX * board.n + startY] = true;
    q.push({startX, startY, 0});
    // Run BFS
    while (!q.empty())
    {
        // Get the front element from the queue
        Point p = q.front();
        q.pop();
        // If we reach the target area, return the depth of the search
        if (isInTargetArea(p.x, p.y, targetArea))
        {
            return p.depth;
        }
        // Move the knight to all valid cells in L-shape and enqueue them
        for (int i = 0; i < 8; ++i)
        {
            int newX = p.x + dx[i];
            int newY = p.y + dy[i];
            // If the new cell is within the board bounds, not visited
            if (isInBoard(newX, newY, board) && !visited[newX * board.n + newY])
            {
                // Mark the new cell as visited and enqueue it with depth incremented by 1
                visited[newX * board.n + newY] = true;
                q.push({newX, newY, p.depth + 1});
            }
        }
    }
    // If the target area is unreachable, return a large value
    return INT_MAX;
}

int upperBoundBFS(const Board &board, int startX, int startY, Rectangle targetArea)
{
    // Create a visited array to mark visited cells using 1D array
    vector<bool> visited(board.m * board.n, false);
    // Create a queue for BFS
    queue<Point> q;
    // Mark the starting point as visited and enqueue it
    visited[startX * board.n + startY] = true;
    q.push({startX, startY, 0});
    // Define possible moves for a knight (L-shape)
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
    // Initialize variables to track the furthest cell found and its depth
    Point furthestCell = {startX, startY, 0};
    int maxDepth = 0;
    // Run BFS
    while (!q.empty())
    {
        // Get the front element from the queue
        Point p = q.front();
        q.pop();
        // Update the furthest cell if the current cell is farther away
        if (p.depth > furthestCell.depth && isInTargetArea(p.x, p.y, targetArea))
        {
            furthestCell = p;
            maxDepth = p.depth;
        }
        // If we reach the target area, return the depth of the search
        // Move the knight to all valid cells in L-shape and enqueue them
        for (int i = 0; i < 8; ++i)
        {
            int newX = p.x + dx[i];
            int newY = p.y + dy[i];
            // If the new cell is within the board bounds, not visited
            if (isInBoard(newX, newY, board) && !visited[newX * board.n + newY])
            {
                // Mark the new cell as visited and enqueue it with depth incremented by 1
                visited[newX * board.n + newY] = true;
                q.push({newX, newY, p.depth + 1});
            }
        }
    }
    // Return the depth of the furthest cell found
    return maxDepth;
}

// Function to compute the lower bound of the state of the board using BFS
int computeLowerBoundBFS(const Board &board)
{
    int lowerBound = 0;

    // Iterate over each knight
    for (int i = 0; i < board.m * board.n; ++i)
    {
        if (board.boardState[i] == 'W')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            // Compute the number of moves to reach the closest board.black cell
            int closestMoves = INT_MAX;
            int moves = bfs(board, knightRow, knightCol, board.black);
            closestMoves = min(closestMoves, moves);
            lowerBound += closestMoves;
        }
        if (board.boardState[i] == 'B')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            // Compute the number of moves to reach the closest board.white cell
            int closestMoves = INT_MAX;
            int moves = bfs(board, knightRow, knightCol, board.white);
            closestMoves = min(closestMoves, moves);
            lowerBound += closestMoves;
        }
    }
    return lowerBound;
}

int computeUpperBoundBFS(const Board &board)
{
    int upperBound = 0;
    // Iterate over each knight
    for (int i = 0; i < board.m * board.n; ++i)
    {
        if (board.boardState[i] == 'W')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            // Compute the number of moves to reach the farthest board.black cell
            int farthestMoves = 0;
            int moves = upperBoundBFS(board, knightRow, knightCol, board.black);
            farthestMoves = max(farthestMoves, moves);
            upperBound += farthestMoves;
        }
        if (board.boardState[i] == 'B')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            // Compute the number of moves to reach the farthest board.white cell
            int farthestMoves = 0;
            int moves = upperBoundBFS(board, knightRow, knightCol, board.white);
            farthestMoves = max(farthestMoves, moves);
            upperBound += farthestMoves;
        }
    }
    return upperBound;
}

bool isSolution(Board board)
{
    for (int i = 0; i < board.m * board.n; ++i)
    {
        if (board.boardState[i] == 'W')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            if (!isInTargetArea(knightRow, knightCol, board.black))
            {
                return false;
            }
        }
        if (board.boardState[i] == 'B')
        {
            int knightRow = i / board.n;
            int knightCol = i % board.n;
            if (!isInTargetArea(knightRow, knightCol, board.white))
            {
                return false;
            }
        }
    }
    return true;
}
void dfs(Board board, int currentDepth, int &bestDepth, bool isWhiteTurn)
{
    if (recursionCounter % 100000 == 0)
    {
        cout << "Recursion counter: " << recursionCounter << endl;
    }
    recursionCounter++;
    int currentLowerBound = computeLowerBoundBFS(board);

    // test if current depth + current lower bound >= bestDepth
    if (currentDepth + currentLowerBound >= bestDepth)
    {
        return;
    }
    if (isSolution(board) && currentDepth < bestDepth)
    {
        bestDepth = currentDepth;
        return;
    }
    vector<Board> validMoves;
    // if it is white turn generate all valid moves for white from the current state
    if (isWhiteTurn)
    {
        for (int i = 0; i < board.m * board.n; ++i)
        {
            if (board.boardState[i] == 'W')
            {
                int knightRow = i / board.n;
                int knightCol = i % board.n;
                // Compute all possible valid chess moves for the white knight and store them in a vector
                for (int j = 0; j < 8; ++j)
                {
                    int newX = knightRow + dx[j];
                    int newY = knightCol + dy[j];
                    if (isInBoard(newX, newY, board) && board.boardState[newX * board.n + newY] == '-')
                    {
                        Board newBoard = board;
                        newBoard.boardState[i] = '-';
                        newBoard.boardState[newX * board.n + newY] = 'W';
                        validMoves.push_back(newBoard);
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < board.m * board.n; ++i)
        {
            if (board.boardState[i] == 'B')
            {
                int knightRow = i / board.n;
                int knightCol = i % board.n;
                // Compute all possible valid chess moves for the white knight and store them in a vector
                for (int j = 0; j < 8; ++j)
                {
                    int newX = knightRow + dx[j];
                    int newY = knightCol + dy[j];
                    if (isInBoard(newX, newY, board) && board.boardState[newX * board.n + newY] == '-')
                    {
                        Board newBoard = board;
                        newBoard.boardState[i] = '-';
                        newBoard.boardState[newX * board.n + newY] = 'B';
                        validMoves.push_back(newBoard);
                    }
                }
            }
        }
    }
    // Sort the valid moves based on the lower bound from the lowest to the highest
    sort(validMoves.begin(), validMoves.end(), [](const Board &a, const Board &b)
         { return computeLowerBoundBFS(a) < computeLowerBoundBFS(b); });

    // Call the dfs function recursively for each valid move
    for (int i = 0; i < validMoves.size(); ++i)
    {
        if (isWhiteTurn)
        {

            dfs(validMoves[i], currentDepth + 1, bestDepth, false);
        }
        else
        {
            dfs(validMoves[i], currentDepth + 1, bestDepth, true);
        }
    }
}

int solve(Board board)
{
    int upperBound = computeUpperBoundBFS(board);
    int bestDepth = upperBound + 1; // Initialize the best depth with the upper bound
    dfs(board, 0, bestDepth, true); // Call the depth-first search function
    return bestDepth;               // Return the best depth found
}

int main()
{
    string filename = "in_0016.txt";
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Unable to open file: " << filename << endl;
        return 1;
    }

    int m, n, k;
    // Read chessboard size and number of figures
    file >> m >> n >> k;
    file >> k;
    cout << "Size of chessboard: " << m << "x" << n << endl;
    cout << "k value: " << k << endl;

    // Read coordinates and create both rectangles
    Rectangle white, black;
    file >> white.x1 >> white.y1 >> white.x2 >> white.y2;
    file >> black.x1 >> black.y1 >> black.x2 >> black.y2;

    Board board = createBoard(m, n, white, black);

    printBoard(board);
    int lowerBoundBFS = computeLowerBoundBFS(board);
    cout << "Lower bound using BFS: " << lowerBoundBFS << endl;
    cout << "Upper bound using BFS: " << computeUpperBoundBFS(board) << endl;

    int bestDepth = solve(board); // Call the solve function to find the best depth
    cout << "Best depth found: " << bestDepth << endl;
    cout << "Recursion counter: " << recursionCounter << endl;
    file.close();
    return 0;
}
