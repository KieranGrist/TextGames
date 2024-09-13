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

	enum Direction
	{
		Error,
		Up,
		Down,
		Left,
		Right
	};

	struct Location
	{
		Location() {}
		Location(char InColumn, int InRow)
		{
			Column = InColumn;
			Row = InRow;
		}
		void PrintLocation() const
		{
			cout << Column << Row;
		}

		bool operator==(const Location& other) const
		{
			return Column == other.Column && Row == other.Row;
		}

		char Column = '!';  // Column label
		int Row = -1;       // Row number
	};

	struct Tile
	{
		Tile()
		{
			SlotState = Error;
			GridLocation = Location();
		}

		void PrintTile() const
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

		static Direction GetDirection(const Location& InStart, const Location& InEnd)
		{
			if (InStart.Column > InEnd.Column)
			{
				return Left;
			}
			else if (InStart.Column < InEnd.Column)
			{
				return Right;
			}
			else if (InStart.Row > InEnd.Row)
			{
				return Up;
			}
			else if (InStart.Row < InEnd.Row)
			{
				return Down;
			}

			return Error;  // If neither column nor row changed
		}

		void PrintDirection(const Location& InStart, const Location& InEnd) const
		{
			cout << GetDirection(InStart, InEnd);
		}


		bool PossibleTileMoves() const
		{
			if (SlotState == Blocked)
				return false;
			if (SlotState == Empty)
				return false;
			int possible = 0;
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
					PrintDirection(GridLocation, neighbour->GridLocation);
					cout << endl;
					possible++;
					break;
				}
			}
			return possible != 0;
		}

		MarbleSlot SlotState;
		Location GridLocation;
		vector<Tile*> CaptureLocations;
	};

	class Board
	{
	private:
		vector<Tile*> Tiles;  // Tiles in the board
		int MaxCol = 7;
		int MaxRow = 7;

	public:
		void PrintValidSelections() const
		{
			for (auto tile : Tiles)
			{
				tile->PossibleTileMoves();
			}
		}

		void Jump(const Location& InJumpStart, const Location& InJumpTo)
		{
			Direction JumpDirection = Tile::GetDirection(InJumpStart, InJumpTo);
			Tile* StartMarble = FindTileByGridLocation(InJumpStart);
			Tile* JumpedMarble = FindTileByGridLocation(InJumpTo);
			Tile* EmptySpace = FindTileByGridLocation(InJumpStart);
		}

		bool SelectTile(const Location& InLocation) const
		{
			Tile* tile = FindTileByGridLocation(InLocation);

			if (!tile)
			{
				InLocation.PrintLocation();
				cout << " is an invalid selection" << endl;
				return false;
			}
			if (!tile->PossibleTileMoves())
			{
				InLocation.PrintLocation();
				cout << " has no possible Jumps/Captures" << endl;
				return false;
			}
			InLocation.PrintLocation();
			cout << " selected" << endl;
			return true;
		}

		Tile* FindTileByGridLocation(const Location& InLocation, Direction InDirection)
		{
			// Additional logic for different directions can be added here
			return FindTileByGridLocation(InLocation);  // Example: Using the default FindTileByGridLocation for now
		}

		Tile* FindTileByGridLocation(const Location& InLocation) const
		{
			return FindByPredicate([InLocation](Tile* t)
				{
					return t->GridLocation == InLocation;
				});
		}

		// Function to find a tile by a predicate
		Tile* FindByPredicate(function<bool(Tile*)> predicate) const
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
					FindTileByGridLocation(Location(tile->GridLocation.Column - 2, tile->GridLocation.Row)),
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

		void PrintBoard() const
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
