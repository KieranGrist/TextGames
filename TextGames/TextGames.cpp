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
	struct Location
	{
		Location() {}
		Location(char InColumn, int InRow)
		{
			Column = InColumn;
			Row = InRow;
		}
		void PrintLocation()
		{
			cout << Column << Row;
		}

		bool operator==(const Location& other) const 
		{
			return Column == other.Column && Row == other.Row;
		}

		char Column = '!';  // Column label
		int Row = -1;         // Row number
	};

	struct Tile
	{
		Tile()
		{
			SlotState = Error;
			GridLocation = Location();
		}

		void PrintTile()
		{
			switch (SlotState)
			{
			case Label:
				if (GridLocation.Column == 64)
				{
					cout << endl;
					cout << GridLocation.Row << " ";
				}
				else
					cout << GridLocation.Column << " ";
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

		bool PossibleTileMoves()
		{
			if (SlotState == Blocked)
				return false;
			if (SlotState == Empty)
				return false;
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
					GridLocation.PrintLocation();
					cout << " Can Jump/Capture ";
					neighbour->GridLocation.PrintLocation();
					cout << endl;
					possibile++;
					break;
				}
			}
			if (possibile != 0)
				return true;
			return false;
		}
		MarbleSlot SlotState;
		Location GridLocation;
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


		void Jump()
		{

		}

		bool SelectTile(Location InLocation)
		{
			Tile* tile = FindTileByGridLocation(InLocation);

			if (!tile)
			{
				InLocation.PrintLocation();
				cout << " Is an invalid selection";
				return false;
			}
			if (!tile->PossibleTileMoves())
			{
				InLocation.PrintLocation();
				cout << " Has no possible Jumps/Captures";
				return false;
			}
			InLocation.PrintLocation();
			cout << " Selected";
		}

		Tile* FindTileByGridLocation(Location InLocation)
		{
			return FindByPredicate([InLocation](Tile* t)
				{
					return t->GridLocation == InLocation;
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
					tile->GridLocation.Column = Col;
					tile->GridLocation.Row = Row;
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
				 FindTileByGridLocation(Location( tile->GridLocation.Column - 2, tile->GridLocation.Row)),
				 FindTileByGridLocation(Location(tile->GridLocation.Column + 2, tile->GridLocation.Row)),
				 FindTileByGridLocation(Location(tile->GridLocation.Column, tile->GridLocation.Row - 2)),
				 FindTileByGridLocation(Location(tile->GridLocation.Column, tile->GridLocation.Row + 2))
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