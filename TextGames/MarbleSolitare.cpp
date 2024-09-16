#include "MarbleSolitare.h"

Location::Location(char InColumn, int InRow)
{
	Column = InColumn;
	Row = InRow;
}

Location::Location(const string& InLocation)
{
	// Check if the input string has the expected length
	if (InLocation.length() >= 2)
	{
		Column = InLocation[0];  // First character is the column (e.g., 'D')
		Row = InLocation[1] - '0';  // Second character is the row (e.g., '4')
	}
	else
	{
		// Handle error: invalid location string
		Column = '!';  // Or any default/error value
		Row = -1;      // Or any default/error value
	}
}

void Location::PrintLocation() const
{
	cout << "Location:" << Column << Row;
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

const MarbleSlot& Tile::GetSlotState() const
{
	return SlotState;
}

void Tile::SetSlotState(const MarbleSlot& InMarbleSlot)
{
	SlotState = InMarbleSlot;
}

const Location& Tile::GetGridLocation() const
{
	return GridLocation;
}

void Tile::SetGridLocation(const Location& InGridLocation)
{
	GridLocation = InGridLocation;
}

void Tile::ClearPossibleJumpDirections()
{
	PossibleJumpDirections.clear();
}

void Tile::AddPossibleJumpDirection(const Direction& InDirection)
{
	PossibleJumpDirections.push_back(InDirection);
}

const vector<Direction>& Tile::GetPossibleJumpDirections() const
{
	return PossibleJumpDirections;
}

bool Tile::HasPossiblePossibleJumpDirection() const
{
	return PossibleJumpDirections.size() > 0;
}

void Tile::PrintDirection(const Direction& InDirection) const
{
	cout << "Direction: ";
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

void Tile::PrintPossibleMoves() const
{
	if (SlotState != MarbleSlot::Marble)
		return;
	for (auto direction : PossibleJumpDirections)
	{
		GridLocation.PrintLocation();
		cout << " Can Jump/Capture ";
		PrintDirection(direction);
		cout << endl;
	}
}

void MarbleBoard::PrintValidSelections() const
{
	for (auto tile : Tiles)
	{
		tile->PrintPossibleMoves();
	}
}

void MarbleBoard::Jump(const Location& InJumpStart, const Direction& InJumpDirection)
{
	Tile* start_marble = FindTileByGridLocation(InJumpStart);
	Tile* jumped_marble = FindTileByDirection(InJumpStart, InJumpDirection, 1);
	Tile* empty_space = FindTileByDirection(InJumpStart, InJumpDirection, 2);

	if (!start_marble || !jumped_marble || !empty_space)
	{
		return;
	}
	start_marble->SetSlotState(MarbleSlot::Empty);
	jumped_marble->SetSlotState(MarbleSlot::Empty);
	empty_space->SetSlotState(MarbleSlot::Marble);
}

bool MarbleBoard::SelectTile(const Location& InLocation) const
{
	Tile* tile = FindTileByGridLocation(InLocation);

	if (!tile)
	{
		InLocation.PrintLocation();
		cout << " is an invalid selection" << endl;
		return false;
	}
	if (!tile->HasPossiblePossibleJumpDirection())
	{
		InLocation.PrintLocation();
		cout << " has no possible Jumps/Captures" << endl;
		return false;
	}
	InLocation.PrintLocation();
	cout << " selected" << endl;
	return true;
}

Tile* MarbleBoard::FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const
{
	switch (InDirection)
	{
	case Direction::DirectionError:
		break;
	case Direction::Up:
		InLocation.Row-= InDistance;
		break;
	case Direction::Down:
		InLocation.Row+= InDistance;
		break;
	case Direction::Left:
		InLocation.Column-= InDistance;
		break;
	case Direction::Right:
		InLocation.Column+= InDistance;
		break;
	default:
		break;
	}
	return FindTileByGridLocation(InLocation);  // Example: Using the default FindTileByGridLocation for now
}

Tile* MarbleBoard::FindTileByGridLocation(const Location& InLocation) const
{
	return FindByPredicate([InLocation](Tile* t)
		{
			return t->GetGridLocation() == InLocation;
		});
}

// Function to find a tile by a predicate
Tile* MarbleBoard::FindByPredicate(function<bool(Tile*)> predicate) const
{
	auto it = find_if(Tiles.begin(), Tiles.end(), predicate);
	if (it != Tiles.end())
		return *it;
	return nullptr;  // Return nullptr if no tile is found
}

void MarbleBoard::GenerateBoard()
{
	// Set up the cross-shaped Marble Solitaire board
	for (int x = 0; x < MaxRow + 1; x++)
	{
		char Col = 64;
		for (int y = 0; y < MaxCol + 1; y++)
		{
			Tile* tile = new Tile();
			Tiles.push_back(tile);

			int row = x;
			string key = Col + to_string(row);

			// Set row and column labels
			Location grid_location;
			grid_location.Column = Col;
			grid_location.Row = row;
			tile->SetGridLocation(grid_location);
			Col++;

			if (x == 0 && y == 0)
			{
			tile->SetSlotState(MarbleSlot::Blocked);
				continue;
			}
			if (x == 0)
			{
				tile->SetSlotState(MarbleSlot::Label);
				continue;
			}
			if (y == 0)
			{
				tile->SetSlotState(MarbleSlot::Label);
				continue;
			}

			// Block the corners (outside the cross shape)
			if ((x < 3 && y < 3) || (x < 3 && y > 5) || (x > 5 && y < 3) || (x > 5 && y > 5))
			{
				tile->SetSlotState(MarbleSlot::Blocked);
			}
			else
			{
				// Place marbles everywhere except the center
				if (x == 4 && y == 4)
					tile->SetSlotState(MarbleSlot::Empty);
				else
					tile->SetSlotState(MarbleSlot::Marble);
			}

		}
	}
}

void MarbleBoard::GeneratePossibleJumpDirection()
{
	cout << endl << "Possible Jumps " << endl;
	for (auto tile : Tiles)
	{
		if (tile->GetSlotState() != MarbleSlot::Marble)
			continue;
		tile->ClearPossibleJumpDirections();
		auto jump_directions =
		{
			Direction::Up,
			Direction::Down,
			Direction::Left,
			Direction::Right
		};
		for (auto current_direction : jump_directions)
		{
			Tile* jumped_marble = FindTileByDirection(tile->GetGridLocation(), current_direction, 1);
			Tile* empty_space = FindTileByDirection(tile->GetGridLocation(), current_direction, 2);

			if (!jumped_marble || jumped_marble->GetSlotState() != MarbleSlot::Marble)
				continue;

			if (!empty_space || empty_space->GetSlotState() != MarbleSlot::Empty)
				continue;

			tile->AddPossibleJumpDirection(current_direction);
		}
	}
}

void MarbleBoard::PrintBoard() const
{
	// Print the board with row numbers
	for (auto tile : Tiles)
	{
		tile->PrintTile();
	}
	cout << endl;
}

int MarbleBoard::MarbleCount() const
{
	int count = 0;
	for (auto tile : Tiles)
	{
		if (tile->GetSlotState() == MarbleSlot::Marble)
			count++;
	}
	return count;
}

void MarbleSolitare::BeginPlay()
{
	EmptyScreen();
	Board.GenerateBoard();
}

void MarbleSolitare::Update()
{
	EmptyScreen();
	Board.PrintBoard();
	Board.GeneratePossibleJumpDirection();
	Board.PrintValidSelections();
	cout << "Select Marble by typing Location then desired jump Direction" <<endl;
	string text_input;
	cin >> text_input;
	SanitiseInput(text_input);

	Location MarbleLocation;
	Direction JumpDirection;
	SeperateLocationAndInput(text_input, MarbleLocation, JumpDirection);
	if (!Board.SelectTile(MarbleLocation))
		return;
	Board.Jump(MarbleLocation, JumpDirection);
	Board.PrintBoard();
	cout << "Jump Completed type anything to continue" << endl;
	text_input = "";
	cin >> text_input;
	text_input = "";
}

void MarbleSolitare::SanitiseInput(string& InInput)
{
	// Use std::transform to convert each character to uppercase
	transform(InInput.begin(), InInput.end(), InInput.begin(), ::toupper);

	// Remove spaces
	InInput.erase(remove_if(InInput.begin(), InInput.end(), ::isspace), InInput.end());
}

void MarbleSolitare::SeperateLocationAndInput(const string& InInput, Location& OutLocation, Direction& OutDirection)
{
	// Assume the location part is the first two characters (e.g., 'D4')
	if (InInput.length() >= 2)
	{
		OutLocation = Location(InInput.substr(0, 2));  // Extract the first two characters as location
	}

	// The remaining part is the direction (e.g., 'RIGHT')
	string directionStr;
	if (InInput.length() > 2)
	{
		directionStr = InInput.substr(2);  // Extract from the 3rd character to the end as direction
	}

	// Convert the direction string into the Direction enum
	if (directionStr == "UP")
	{
		OutDirection = Direction::Up;
	}
	else if (directionStr == "DOWN")
	{
		OutDirection = Direction::Down;
	}
	else if (directionStr == "LEFT")
	{
		OutDirection = Direction::Left;
	}
	else if (directionStr == "RIGHT")
	{
		OutDirection = Direction::Right;
	}
	else
	{
		OutDirection = Direction::DirectionError;  // Default case for invalid input
	}
}

bool MarbleSolitare::IsPlaying()
{
	return Board.MarbleCount() > 1;
}

void MarbleSolitare::EmptyScreen()
{
#ifdef _WIN32
	system("cls");  // Windows
#else
	system("clear");  // Linux/Unix/OSX
#endif
}
