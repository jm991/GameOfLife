#include <iostream>
#include <unordered_map>
#include <sstream> // for std::stringstream

static const int NUM_ITERATIONS = 10;
static  const std::string FILE_HEADER = "#Life 1.06";

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

    unordered_map<pair<int, int>, bool, PairHash> Board;

    string Line;
    bool bValidFormat = true;

    // Make sure the input starts with the Life 1.06 header
    if (getline(cin, Line) && Line != FILE_HEADER)
    {
        return 1;
    }

    // Read input and parse pairs into the starting board
    while (getline(cin, Line) && bValidFormat)
    {
        int X, Y;
        stringstream ss(Line);
        if (ss >> X >> Y)
        {
            // Mark live cells
            Board[{X, Y}] = true;
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
        simulateGameOfLife(Board);
    }

    // Print the final board state
    printBoard(Board);

    return 0;
}
