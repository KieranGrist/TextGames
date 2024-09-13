#include "MarbleSolitare.h"

Location::Location(char InColumn, int InRow)
{
	Column = InColumn;
	Row = InRow;
}
void Location::PrintLocation() const
{
	cout << Column << Row;
}
Tile::Tile()
{
	SlotState = MarbleSlot::SlotError;
	GridLocation = Location();
}

void Tile::PrintTile() const
{
	switch (SlotState)
	{
	case  MarbleSlot::Label:
		if (GridLocation.Column == 64)
		{
			cout << endl;
			cout << GridLocation.Row << " ";
		}
		else
			cout << GridLocation.Column << " ";
		break;
	case  MarbleSlot::Marble:
		cout << "O ";  // Marble
		break;
	case  MarbleSlot::Empty:
		cout << "X ";  // Empty
		break;
	case  MarbleSlot::Blocked:
		cout << "  ";  // Blocked (no character for empty space)
		break;
	case  MarbleSlot::SlotError:
		cout << "? ";  // Error
		break;
	}
};

Direction Tile::GetDirection(const Location& InStart, const Location& InEnd)
{
	if (InStart.Column > InEnd.Column)
	{
		return Direction::Left;
	}
	else if (InStart.Column < InEnd.Column)
	{
		return Direction::Right;
	}
	else if (InStart.Row > InEnd.Row)
	{
		return Direction::Up;
	}
	else if (InStart.Row < InEnd.Row)
	{
		return Direction::Down;
	}

	return Direction::DirectionError;  // If neither column nor row changed
}

void Tile::PrintDirection(const Direction& InDirection) const
{
	switch (InDirection)
	{
	case Direction::DirectionError:
		break;
	case Direction::Up:
		cout << "Up";
		break;
	case Direction::Down:
		cout << "Down";
		break;
	case Direction::Left:
		cout << "Left";
		break;
	case Direction::Right:
		cout << "Right";
		break;
	default:
		break;
	}
}

bool Tile::PossibleTileMoves() const
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
		case MarbleSlot::Label:
		case MarbleSlot::Blocked:
		case MarbleSlot::SlotError:
		case MarbleSlot::Marble:
			continue;
			break;
		case MarbleSlot::Empty:
			GridLocation.PrintLocation();
			cout << " Can Jump/Capture ";
			PrintDirection(GetDirection(GridLocation, neighbour->GridLocation));
			cout << endl;
			possible++;
			break;
		}
	}
	return possible != 0;
}

void Board::PrintValidSelections() const
{
	for (auto tile : Tiles)
	{
		tile->PossibleTileMoves();
	}
}

void Board::Jump(const Location& InJumpStart, const Location& InJumpTo)
{
	Direction JumpDirection = Tile::GetDirection(InJumpStart, InJumpTo);
	Tile* StartMarble = FindTileByGridLocation(InJumpStart);
	Tile* JumpedMarble = FindTileByDirection(InJumpStart, JumpDirection);
	Tile* EmptySpace = FindTileByGridLocation(InJumpStart);

	if (!StartMarble || !JumpedMarble || !EmptySpace)
	{
		return;
	}
}

bool Board::SelectTile(const Location& InLocation) const
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

Tile* Board::FindTileByDirection(Location InLocation, Direction InDirection)
{
	switch (InDirection)
	{
	case Direction::DirectionError:
		break;
	case Direction::Up:
		InLocation.Row--;
		break;
	case Direction::Down:
		InLocation.Row++;
		break;
	case Direction::Left:
		InLocation.Column--;
		break;
	case Direction::Right:
		InLocation.Column++;
		break;
	default:
		break;
	}
	return FindTileByGridLocation(InLocation);  // Example: Using the default FindTileByGridLocation for now
}

Tile* Board::FindTileByGridLocation(const Location& InLocation) const
{
	return FindByPredicate([InLocation](Tile* t)
		{
			return t->GridLocation == InLocation;
		});
}

// Function to find a tile by a predicate
Tile* Board::FindByPredicate(function<bool(Tile*)> predicate) const
{
	auto it = find_if(Tiles.begin(), Tiles.end(), predicate);
	if (it != Tiles.end())
		return *it;
	return nullptr;  // Return nullptr if no tile is found
}

void Board::GenerateBoard()
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

void Board::GenerateCaptureLocations()
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

void Board::PrintBoard() const
{
	// Print the board with row numbers
	for (auto tile : Tiles)
	{
		tile->PrintTile();
	}
	cout << endl;
}
