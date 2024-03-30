#include <iostream>
#include <unordered_map>
#include <sstream> // for std::stringstream

using namespace std;

// Define a hash function for pairs of integers (coordinates)
struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

// Function to count live neighbors of a cell
int countLiveNeighbors(int x, int y, const unordered_map<pair<int, int>, bool, PairHash>& board)
{
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) // Offset coordinates in cardinal directions
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0) // Cell skips its own coordinate
            {
                continue;
            }

            // Check if the cell exists in the board
            auto it = board.find({ x + dx, y + dy });
            if (it != board.end() && it->second)
            {
                count++;
            }
        }
    }
    return count;
}

// Function to simulate one iteration of Game of Life
void simulateGameOfLife(unordered_map<pair<int, int>, bool, PairHash>& board)
{
    unordered_map<pair<int, int>, bool, PairHash> newBoard;
    for (auto& cell : board)
    {
        int x = cell.first.first;
        int y = cell.first.second;
        int liveNeighbors = countLiveNeighbors(x, y, board);
        if (cell.second)
        { // if cell is alive
            if (liveNeighbors < 2 || liveNeighbors > 3)
            {
                newBoard[{x, y}] = false; // cell dies
            }
            else
            {
                newBoard[{x, y}] = true; // cell survives
            }
        }
        else
        { // if cell is dead
            if (liveNeighbors == 3)
            {
                newBoard[{x, y}] = true; // cell becomes alive
            }
        }
    }
    board = newBoard; // update the board
}

// Function to print the board in Life 1.06 format
void printBoard(unordered_map<pair<int, int>, bool, PairHash>& board)
{
    for (auto& cell : board)
    {
        if (cell.second)
        {
            cout << cell.first.first << " " << cell.first.second << endl;
        }
    }
}

int main()
{
    cout << "Game of Life" << endl;

    // Read input
    unordered_map<pair<int, int>, bool, PairHash> board;
    string line;
    while (getline(cin, line))
    {
        int x, y;
        stringstream ss(line);
        if (ss >> x >> y)
        {
            board[{x, y}] = true; // mark live cells
        }
        else
        {
            break;
        }
    }

    // Simulate 10 iterations of Game of Life
    for (int i = 0; i < 10; ++i)
    {
        simulateGameOfLife(board);
    }

    // Print the final board state
    printBoard(board);

    return 0;
}
