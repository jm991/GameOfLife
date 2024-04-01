#include <iostream>
#include <unordered_map>
#include <sstream> // for std::stringstream

static const int NUM_ITERATIONS = 10;
static  const std::string FILE_HEADER = "#Life 1.06";

using namespace std;

// Define a hash function for pairs of integers (cell coordinates)
struct PairHash
{
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

    // Offset delta coordinates in 8 cardinal directions
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            // Cell skips its own coordinate
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            // Check if the cell exists in the board and is alive
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
        const int x = cell.first.first;
        const int y = cell.first.second;

        const int liveNeighbors = countLiveNeighbors(x, y, board);

        // if cell is alive
        if (cell.second)
        {
            // Any live cell with fewer than two live neighbors dies, as if by underpopulation.
            // Any live cell with more than three live neighbors dies, as if by overpopulation.
            if (liveNeighbors < 2 || liveNeighbors > 3)
            {
                // cell dies
                newBoard[{x, y}] = false;
            }
            else // Any live cell with two or three live neighbors lives on to the next generation.
            {
                // cell survives
                newBoard[{x, y}] = true;
            }
        }
        else // if cell is dead
        {
            // Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
            if (liveNeighbors == 3)
            {
                // cell becomes alive
                newBoard[{x, y}] = true;
            }
        }
    }

    // update the board
    // @TODO: swap inside of the same board to save memory and not reallocate the map
    board = newBoard;
}

// Function to print the board in Life 1.06 format
void printBoard(const unordered_map<pair<int, int>, bool, PairHash>& board)
{
    printf("Result of %d iterations:\n", NUM_ITERATIONS);

    cout << FILE_HEADER << endl;

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
    cout << "Enter starting board in the Life 1.06 format..." << endl;

    unordered_map<pair<int, int>, bool, PairHash> board;

    string line;
    bool bValidFormat = true;

    // Make sure the input starts with the Life 1.06 header
    if (getline(cin, line) && line != FILE_HEADER)
    {
        return 1;
    }

    // Read input and parse pairs into the starting board
    while (getline(cin, line) && bValidFormat)
    {
        int X, Y;
        stringstream ss(line);
        if (ss >> X >> Y)
        {
            // Mark live cells
            board[{X, Y}] = true;
        }
        else
        {
            // End of user input, or invalid pair
            bValidFormat = false;
            break;
        }
    }

    // Simulate iterations of Game of Life
    for (int i = 0; i < NUM_ITERATIONS; ++i)
    {
        simulateGameOfLife(board);
    }

    // Print the final board state
    printBoard(board);

    return 0;
}
