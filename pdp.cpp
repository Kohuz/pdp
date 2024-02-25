#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>
#include <cmath>
#include <queue>

using namespace std;

struct Rectangle
{
    int x1, y1, x2, y2;
};

struct Point
{
    int x;
    int y;
};

struct Board
{
    int m; // Number of rows
    int n; // Number of columns
    vector<char> boardState;
    Rectangle white;
    Rectangle black;
};

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
void moveKnight(Board &board, int startX, int startY)
{
    // Define possible moves for a knight (L-shape)
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    // Check if the move is valid
    bool validMove = false;
    int newX = 0;
    int newY = 0;
    for (int i = 0; i < 8; ++i)
    {
        int newX = startX + dx[i];
        int newY = startY + dy[i];
        if (newX >= 0 && newX < board.m && newY >= 0 && newY < board.n && (board.boardState[newX * board.n + newY] == '-'))
        {
            cout << "OldX " << startX << endl;
            cout << "OldY " << startY << endl;
            cout << "NewX " << newX << endl;
            cout << "NewY " << newY << endl;

            validMove = true;
            break;
        }
    }

    if (validMove)
    {
        board.boardState[startX * board.n + startY] = '-';
        board.boardState[newX * board.n + newY] = 'W';
    }
    else
    {
        exit(2);
    }
}

// int bfs(const Board &board, int startX, int startY, int targetX, int targetY)
// {
//     // Create a visited array to mark visited cells
//     vector<vector<bool>> visited(board.m, vector<bool>(board.n, false));

//     // Create a queue for BFS
//     queue<Point> q;

//     // Mark the starting point as visited and enqueue it
//     visited[startX][startY] = true;
//     q.push({startX, startY, 0});

//     // Define possible moves for a knight (L-shape)
//     int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
//     int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

//     // Run BFS
//     while (!q.empty())
//     {
//         // Get the front element from the queue
//         Point p = q.front();
//         q.pop();

//         // If we reach the target area, return the number of moves
//         if (p.x == targetX && p.y == targetY)
//         {
//             return p.dist;
//         }

//         // Move the knight to all valid cells in L-shape and enqueue them with distance incremented by 1
//         for (int i = 0; i < 8; ++i)
//         {
//             int newX = p.x + dx[i];
//             int newY = p.y + dy[i];

//             // If the new cell is within the board bounds, not visited
//             if (newX >= 0 && newX < board.m && newY >= 0 && newY < board.n &&
//                 !visited[newX][newY])
//             {
//                 // Mark the new cell as visited and enqueue it with distance incremented by 1
//                 visited[newX][newY] = true;
//                 q.push({newX, newY, p.dist + 1});
//             }
//         }
//     }

//     // If the target area is unreachable, return a large value
//     return INT_MAX;
// }
int bfs(const Board &board, int startX, int startY, Rectangle targetArea)
{
    // Create a visited array to mark visited cells using 1D array
    vector<bool> visited(board.m * board.n, false);
    // Create a queue for BFS
    queue<pair<Point, int>> q; // Store the point and its depth
    // Mark the starting point as visited and enqueue it
    visited[startX * board.n + startY] = true;
    q.push({{startX, startY}, 0});
    // Define possible moves for a knight (L-shape)
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
    // Run BFS
    while (!q.empty())
    {
        // Get the front element from the queue
        pair<Point, int> p = q.front();
        q.pop();
        // If we reach the target area, return the depth of the search
        if (p.first.x >= targetArea.x1 && p.first.x <= targetArea.x2 && p.first.y >= targetArea.y1 && p.first.y <= targetArea.y2)
        {
            return p.second;
        }
        // Move the knight to all valid cells in L-shape and enqueue them
        for (int i = 0; i < 8; ++i)
        {
            int newX = p.first.x + dx[i];
            int newY = p.first.y + dy[i];
            // If the new cell is within the board bounds, not visited
            if (newX >= 0 && newX < board.m && newY >= 0 && newY < board.n && !visited[newX * board.n + newY])
            {
                // Mark the new cell as visited and enqueue it with depth incremented by 1
                visited[newX * board.n + newY] = true;
                q.push({{newX, newY}, p.second + 1});
            }
        }
    }
    // If the target area is unreachable, return a large value
    return INT_MAX;
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
            cout << "Closest moves for white knight at (" << knightRow << ", " << knightCol << "): " << closestMoves << endl;
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
            cout << "Closest moves for black knight at (" << knightRow << ", " << knightCol << "): " << closestMoves << endl;
            lowerBound += closestMoves;
        }
    }
    return lowerBound;
}

int main()
{
    string filename = "in_0010.txt";
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

    file.close();
    return 0;
}
