#include <iostream>
#include <string>
#include <map>
using namespace std;

namespace MarbleSolitaire
{
    enum MarbleSlot
    {
        Error,
        Marble,
        Empty,
        Blocked
    };

    struct Tile
    {
        Tile() : SlotState(Error), Row(-1), Column("!"), Label("") {}
        MarbleSlot SlotState;
        string Column;  // Column label
        int Row;        // Row number
        string Label;
    };

    class Board
    {
    private:
        map<string, Tile> TileMap;  // String key (like "A1") and Tile value
        int MaxRow = 7;  // Number of playable rows (1-7)
        int MaxCol = 7;  // Number of playable columns (A-G)

    public:
        void GenerateBoard()
        {
            // Set up the cross-shaped Marble Solitaire board without label tiles
            for (int x = 0; x < MaxRow; x++)
            {
                char Col = 'A';  // Reset Col to 'A' at the start of each row

                for (int y = 0; y < MaxCol; y++)
                {
                    Tile tile;
                    int Row = x + 1;

                    // Construct the key from Col and Row (e.g., "A1", "B2", etc.)
                    string key = Col + to_string(Row);

                    // Block the corners (outside the cross shape)
                    if ((x < 2 && y < 2) || (x < 2 && y > 4) || (x > 4 && y < 2) || (x > 4 && y > 4))
                    {
                        tile.SlotState = Blocked;
                    }
                    else
                    {
                        // Place marbles everywhere except the center
                        if (x == 3 && y == 3)
                            tile.SlotState = Empty;
                        else
                            tile.SlotState = Marble;
                    }

                    // Store the tile in the map with its key
                    TileMap[key] = tile;

                    // Increment Col to move to the next column
                    Col++;
                }
            }
        }

        void PrintBoard()
        {
            // Print the column labels
            cout << "   ";  // Offset for row labels
            for (char col = 'A'; col <= 'G'; col++)
            {
                cout << " " << col << " ";  // Print column labels A-G
            }
            cout << endl;

            // Print the board row by row
            for (int x = 1; x <= MaxRow; x++)
            {
                // Print the row label
                cout << " " << x << " ";  // Row number label

                for (char col = 'A'; col < 'A' + MaxCol; col++)
                {
                    string key = string(1, col) + to_string(x);  // Construct the key

                    // Retrieve the tile from the map
                    Tile& tile = TileMap[key];

                    switch (tile.SlotState)
                    {
                    case Marble:
                        cout << " O ";  // Marble
                        break;
                    case Empty:
                        cout << " X ";  // Empty slot
                        break;
                    case Blocked:
                        cout << "   ";  // Blocked area (no character)
                        break;
                    case Error:
                    default:
                        cout << " ? ";  // Error or unknown
                        break;
                    }
                }
                cout << endl;
            }
        }
    };
}

int main()
{
    // Create a board object
    MarbleSolitaire::Board board;

    // Generate and print the board
    board.GenerateBoard();
    board.PrintBoard();

    return 0;
}
