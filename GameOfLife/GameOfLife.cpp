#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

static const int NUM_GENERATIONS = 10;
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
int countLiveNeighbors(int x, int y, const std::unordered_set<std::pair<int, int>, PairHash>& board)
{
    int liveCount = 0;

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

            const int curX = (x + dx);
            const int curY = (y + dy);
            const std::pair<int, int> curCell = { curX, curY };

            // Check if the cell exists in the board (only contains live cells)
            if (board.find(curCell) != board.end())
            {
                liveCount++;
            }
        }
    }

    return liveCount;
}

// Checks if a cell will be alive or not in the next generation
bool isAlive(const pair<pair<int, int>, bool>& cellState, const std::unordered_set<std::pair<int, int>, PairHash>& board)
{
    const int x = cellState.first.first;
    const int y = cellState.first.second;
    const bool bIsAlive = cellState.second;

    const int liveNeighbors = countLiveNeighbors(x, y, board);

    // If cell is alive
    if (bIsAlive)
    {
        // Any live cell with fewer than two live neighbors dies, as if by underpopulation.
        // Any live cell with more than three live neighbors dies, as if by overpopulation.
        if (liveNeighbors < 2 || liveNeighbors > 3)
        {
            // Cell dies
            return false;
        }
        else // Any live cell with two or three live neighbors lives on to the next generation.
        {
            // Cell survives
            return true;
        }
    }
    else // If cell is dead
    {
        // Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
        if (liveNeighbors == 3)
        {
            // Cell becomes alive
            return true;
        }
    }

    return false;
}

// Helper function to construct a pair of cell + its alive/dead state using the board of live cells
std::pair<std::pair<int, int>, bool> getCurrentCellState(const std::unordered_set<std::pair<int, int>, PairHash>& board, const std::pair<int, int>& cell)
{
    const bool bIsAlive = (board.find(cell) != board.end()); // If the cell is in the board, it's alive
    return std::make_pair(cell, bIsAlive);
}

// Function to simulate one generation of Game of Life
std::unordered_set<std::pair<int, int>, PairHash> simulateGameOfLife(const std::unordered_set<std::pair<int, int>, PairHash>& board)
{
    // If there's a cell in this new board, it's a live cell
    std::unordered_set<std::pair<int, int>, PairHash> newBoard;

    // Keep a cache of the alive/dead cells we've already tested during this simulation step, to avoid retesting the same cells
    // (common occurrence when testing grid around 2 live cells that are touching)
    unordered_map<pair<int, int>, bool, PairHash> testHistory;

    // Iterate over the live cells in the current board
    for (const std::pair<int, int>& cell : board)
    {
        const int x = cell.first;
        const int y = cell.second;

        // We need to test all of its neighbors too, in case they become alive
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                const int curX = (x + dx);
                const int curY = (y + dy);
                const std::pair<int, int> curCell = { curX, curY };

                const bool bAlreadyTested = (testHistory.find(curCell) != testHistory.end());

                // Avoid testing the same cell twice
                if (!bAlreadyTested)
                {
                    // Always update the testHistory cache
                    const bool bIsCurCellAlive = isAlive(getCurrentCellState(board, curCell), board);
                    testHistory[curCell] = bIsCurCellAlive;

                    // Only add alive cells to the newBoard
                    if (bIsCurCellAlive)
                    {
                        newBoard.insert(curCell);
                    }
                }
            }
        }
    }

    // Return the newly constructed board of alive cells
    return newBoard;
}

// Function to print the board in Life 1.06 format
void printBoard(const std::unordered_set<std::pair<int, int>, PairHash>& board)
{
    printf("Result of %d generations:\n", NUM_GENERATIONS);

    // Print the Life 1.06 file header
    cout << FILE_HEADER << endl;

    // Print all the cell coordinates
    for (auto& cell : board)
    {
        cout << cell.first << " " << cell.second << endl;
    }
}

int main()
{
    cout << "Game of Life" << endl;
    cout << "Enter starting board in the Life 1.06 format..." << endl;

    // Store only the alive cells in the board as an unordered set (unique entries only, O(1) constant lookup and insertion)
    std::unordered_set<std::pair<int, int>, PairHash> board;

    // Start parsing cin
    string line;
    bool bValidFormat = true;

    // Make sure the input starts with the Life 1.06 file header
    if (getline(cin, line) && line != FILE_HEADER)
    {
        return 1;
    }

    // Read input and parse pairs into the starting board
    while (getline(cin, line) && bValidFormat)
    {
        int x, y;
        stringstream ss(line);
        if (ss >> x >> y)
        {
            // Mark live cells
            board.insert(std::make_pair(x, y));
        }
        else
        {
            // End of user input, or invalid pair
            bValidFormat = false;
            break;
        }
    }

    // Simulate iterations of Game of Life
    for (int i = 0; i < NUM_GENERATIONS; ++i)
    {
        // Update the board each iteration
        board = simulateGameOfLife(board);
    }

    // Print the final board state
    printBoard(board);

    return 0;
}
