#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>
#include <cmath>
#include <queue>
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
    // Define possible moves for a knight (L-shape)
    int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};
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
                // Print the move chosen by the BFS
                cout << "Move: (" << p.x << ", " << p.y << ") -> (" << newX << ", " << newY << ")" << endl;
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
            cout << "Farthest moves for white knight at (" << knightRow << ", " << knightCol << "): " << farthestMoves << endl;
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
            cout << "Farthest moves for black knight at (" << knightRow << ", " << knightCol << "): " << farthestMoves << endl;
            upperBound += farthestMoves;
        }
    }
    return upperBound;
}
// int bb_DFS(Board board, int lower_bound, int upperBound)
// {
//     // Create a visited array to mark visited cells using 1D array
//     vector<bool> visited(board.m * board.n, false);
//     // Create a stack for DFS
//     stack<vector<char>> s;
//     // Mark the starting point as visited and push it
//     visited[startX * board.n + startY] = true;
//     s.push({startX, startY, 0});

//     // generate all possible valid moves from this state
// }
// void dfs(Board board, int lower_bound, int upperBound) {
//     // Check termination conditions
//     if (/* Termination condition based on lower bound */ || /* Termination condition based on upper bound */) {
//         // Handle termination
//         return;
//     }

//     // Calculate lower bound for the current state
//     // Update lower bound for subsequent states

//     // Generate all possible moves for white knight
//     // Calculate lower bounds for resulting states
//     // Prune branches based on upper bound

//     // Generate all possible moves for black knight
//     // Calculate lower bounds for resulting states
//     // Prune branches based on upper bound

//     // Recursively explore remaining moves
// }

// int solve(Board board)
// {
//     // Compute the lower bound
//     int lowerBound = computeLowerBoundBFS(board);
//     // Compute the upper bound
//     int upperBound = computeUpperBoundBFS(board);
//     int bb_DFS(board, lower_bound, upperBound);
// }
int main()
{
    string filename = "in_0017.txt";
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
    // int result = solve(board);

    file.close();
    return 0;
}
