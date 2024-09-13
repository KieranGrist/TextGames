#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  // For std::find_if
#include <functional> // For std::function

using namespace std;

namespace MarbleSolitaire
{
	enum MarbleSlot
	{
		Error,
		Marble,
		Empty,
		Blocked,
		Label
	};

	struct Tile
	{
		Tile()
		{
			SlotState = Error;
			Column = '!';  // Column label
			Row = -1;         // Row number
		}

		void PrintTile()
		{
			switch (SlotState)
			{
			case Label:
				if (Column == 64)
				{
					cout << endl;
					cout << Row << " ";
				}
				else
					cout << Column << " ";
				break;
			case Marble:
				cout << "O ";  // Marble
				break;
			case Empty:
				cout << "X ";  // Empty
				break;
			case Blocked:
				cout << "  ";  // Blocked (no character for empty space)
				break;
			case Error:
				cout << "? ";  // Error
				break;
			}
		};

		void PrintLocation()
		{
			cout << Column << Row;
		}

		bool PossibleTileMoves(bool InPrint = false)
		{
			if (SlotState == Blocked)
				return;
			if (SlotState == Empty)
				return;
			int possibile = 0;
			for (auto neighbour : CaptureLocations)
			{
				switch (neighbour->SlotState)
				{
				case Label:
				case Blocked:
				case Error:
				case Marble:
					continue;
					break;
				case Empty:
					PrintLocation();
					cout << " Can Jump/Capture ";
					neighbour->PrintLocation();
					cout << endl;
					possibile++;
					break;
				}
			}
			if (possibile != 0)
				return true;
			if (InPrint)
			{
				PrintLocation();
				cout << " Has No valid Jumps/Captures";
			}
			return false;
		}
		MarbleSlot SlotState;
		char Column = '!';  // Column label
		int Row = -1;         // Row number
		vector<Tile*> CaptureLocations;
	};

	class Board
	{
	private:
		vector<Tile*> Tiles;  // String key (like "A1") and Tile value
		int MaxCol = 7;
		int MaxRow = 7;
	public:
		void PrintValidSelections()
		{
			for (auto tile : Tiles)
			{
				tile->PossibleTileMoves();
			}
		}


		void SelectTile(char InColumn, int InRow)
		{
			Tile* tile = FindTileByGridLocation(InColumn, InRow);
	
			if (!tile)
			{
				cout << InColumn << InRow << " Is an invalid selection";
				return;
			}
			tile->PossibleTileMoves();
		}
		Tile* FindTileByGridLocation(char InColumn, int InRow)
		{
			return FindByPredicate([InColumn, InRow](Tile* t)
				{
					return t->Column == InColumn && t->Row == InRow;
				});
		}

		// Function to find a tile by a predicate
		Tile* FindByPredicate(function<bool(Tile*)> predicate)
		{
			auto it = find_if(Tiles.begin(), Tiles.end(), predicate);
			if (it != Tiles.end())
				return *it;
			return nullptr;  // Return nullptr if no tile is found
		}

		void GenerateBoard()
		{
			// Set up the cross-shaped Marble Solitaire board
			for (int x = 0; x < MaxRow + 1; x++)
			{
				char Col = 64;
				for (int y = 0; y < MaxCol + 1; y++)
				{
					Tile* tile = new Tile();
					Tiles.push_back(tile);

					int Row = x;
					string key = Col + to_string(Row);

					// Set row and column labels
					tile->Column = Col;
					tile->Row = Row;
					Col++;

					if (x == 0 && y == 0)
					{
						tile->SlotState = Blocked;
						continue;
					}
					if (x == 0)
					{
						tile->SlotState = Label;
						continue;
					}
					if (y == 0)
					{
						tile->SlotState = Label;
						continue;
					}

					// Block the corners (outside the cross shape)
					if ((x < 3 && y < 3) || (x < 3 && y > 5) || (x > 5 && y < 3) || (x > 5 && y > 5))
					{
						tile->SlotState = Blocked;
					}
					else
					{
						// Place marbles everywhere except the center
						if (x == 4 && y == 4)
							tile->SlotState = Empty;
						else
							tile->SlotState = Marble;
					}

				}
			}
			GenerateCaptureLocations();
		}

		void GenerateCaptureLocations()
		{
			for (auto tile : Tiles)
			{
				vector<Tile*> CaptureLocations = {
				 FindTileByGridLocation(tile->Column - 2, tile->Row),
				 FindTileByGridLocation(tile->Column + 2, tile->Row),
				 FindTileByGridLocation(tile->Column, tile->Row - 2),
				 FindTileByGridLocation(tile->Column, tile->Row + 2)
				};
				for (auto neighbour : CaptureLocations)
				{
					if (!neighbour)
						continue;
					tile->CaptureLocations.push_back(neighbour);
				}
			}
		}

		void PrintBoard()
		{
			// Print the board with row numbers
			for (auto tile : Tiles)
			{
				tile->PrintTile();
			}
			cout << endl;
		}
	};
};

void EmptyScreen()
{

}

int main()
{
	// Create a board object
	MarbleSolitaire::Board board;

	// Generate and print the board
	board.GenerateBoard();
	board.PrintBoard();
	board.PrintValidSelections();

	return 0;
}