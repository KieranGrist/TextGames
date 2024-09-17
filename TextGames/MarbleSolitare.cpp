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

Tile::Tile()
{
	SlotState = MarbleSlot::SlotError;
	GridLocation = Location();
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

void Tile::PrintPossibleMoves() const
{
	if (SlotState != MarbleSlot::Marble)
		return;
	for (auto direction : PossibleJumpDirections)
	{
		MarbleSolitare::PrintLocation(GridLocation);
		cout << " Can Jump/Capture ";
		MarbleSolitare::PrintDirection(direction);
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
		MarbleSolitare::PrintLocation(InLocation);
		cout << " is an invalid selection" << endl;
		return false;
	}
	if (!tile->HasPossiblePossibleJumpDirection())
	{
		MarbleSolitare::PrintLocation(InLocation);
		cout << " has no possible Jumps/Captures" << endl;
		return false;
	}
	MarbleSolitare::PrintLocation(InLocation);
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
		InLocation.Row -= InDistance;
		break;
	case Direction::Down:
		InLocation.Row += InDistance;
		break;
	case Direction::Left:
		InLocation.Column -= InDistance;
		break;
	case Direction::Right:
		InLocation.Column += InDistance;
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
			return t->GetGridLocation().Equals(InLocation);
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

const vector<Tile*>& MarbleBoard::GetTiles() const
{
	return Tiles;
}

void MarbleBoard::GenerateBoard()
{
	Tiles.clear();

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

void MarbleBoard::GenerateBoardPossibleJumpDirections()
{
	cout << endl << "Possible Jumps " << endl;
	for (auto tile : Tiles)
	{
		GenerateTilePossibleJumpDirections(tile);
	}
}

void MarbleBoard::GenerateTilePossibleJumpDirections(Tile* InTile)
{
	if (InTile->GetSlotState() != MarbleSlot::Marble)
		return;
	InTile->ClearPossibleJumpDirections();
	auto jump_directions =
	{
		Direction::Up,
		Direction::Down,
		Direction::Left,
		Direction::Right
	};
	for (auto current_direction : jump_directions)
	{
		Tile* jumped_marble = FindTileByDirection(InTile->GetGridLocation(), current_direction, 1);
		Tile* empty_space = FindTileByDirection(InTile->GetGridLocation(), current_direction, 2);

		if (!jumped_marble || jumped_marble->GetSlotState() != MarbleSlot::Marble)
			continue;

		if (!empty_space || empty_space->GetSlotState() != MarbleSlot::Empty)
			continue;

		InTile->AddPossibleJumpDirection(current_direction);
	}
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
	MarbleSolitare::PrintBoard(Board);
	Board.GenerateBoardPossibleJumpDirections();
	Board.PrintValidSelections();
	cout << "Select Marble by typing Location then desired jump Direction" << endl;
	string text_input;
	cin >> text_input;
	SanitiseInput(text_input);

	Location MarbleLocation;
	Direction JumpDirection;
	SeperateLocationAndInput(text_input, MarbleLocation, JumpDirection);
	if (!Board.SelectTile(MarbleLocation))
		return;
	Board.Jump(MarbleLocation, JumpDirection);
	MarbleSolitare::PrintBoard(Board);
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

Direction MarbleSolitare::GetDirection(const Location& InStart, const Location& InEnd)
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

void MarbleSolitare::PrintBoard(const MarbleBoard& InMarbleBoard)
{
	// Print the board with row numbers
	for (auto tile : InMarbleBoard.GetTiles())
	{
		PrintTile(*tile);
	}
	cout << endl;
}

void MarbleSolitare::PrintLocation(const Location& InLocation)
{
	cout << "Location:" << InLocation.Column << InLocation.Row;
}

void MarbleSolitare::PrintTile(const Tile& InTitle)
{
	switch (InTitle.GetSlotState())
	{
	case  MarbleSlot::Label:
		if (InTitle.GetGridLocation().Column == 64)
		{
			cout << endl;
			cout << InTitle.GetGridLocation().Row << " ";
		}
		else
			cout << InTitle.GetGridLocation().Column << " ";
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
}

void MarbleSolitare::PrintDirection(const Direction& InDirection)
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

void MarbleSolitare::PrintSlotState(const MarbleSlot& InMarbleSlot)
{
	switch (InMarbleSlot)
	{
	case  MarbleSlot::Label:
		cout << "Label";
	case  MarbleSlot::Marble:
		cout << "Marble";
		break;
	case  MarbleSlot::Empty:
		cout << "Empty";
		break;
	case  MarbleSlot::Blocked:
		cout << "Blocked";
		break;
	case  MarbleSlot::SlotError:
		cout << "SlotError";
		break;
	}
}

void MarbleSolitare::EmptyScreen()
{
#ifdef _WIN32
	system("cls");  // Windows
#else
	system("clear");  // Linux/Unix/OSX
#endif
}

void MarbleSolitare::SimulateBFS()
{
	MarbleBoard* start_board = new MarbleBoard();
	start_board->GenerateBoard();
	//queue<MarbleBoard*> board_queue;  // Queue for BFS
	AddBoardToQueue(start_board);
	int wins = 0;
	int boards = 0;
	int jumps = 0;
	while (!BoardQueue.empty())
	{
		MarbleBoard* current_board = GetTopBoard();
		boards++;
		// Collect statistics for the current board state
		int marbleCount = current_board->MarbleCount();
		cout << "Number of marbles remaining: " << marbleCount << endl;
		cout << "Boards: " << boards << " Board left: " << BoardQueue.size() << endl;

		// Generate possible moves from the current board state
		for (auto tile : current_board->GetTiles())
		{
			if (tile->GetSlotState() != MarbleSlot::Marble)
				continue;
			current_board->GenerateTilePossibleJumpDirections(tile);
			for (const auto& direction : tile->GetPossibleJumpDirections())
			{
				// Simulate the move and add the new board state to the queue
				MarbleBoard* new_board = current_board->SimulateMove(tile->GetGridLocation(), direction);
				AddBoardToQueue(new_board);
				jumps++;
			}

		}
		if (current_board->MarbleCount() == 1)
			wins++;
	}
	cout << wins << boards << jumps;
}

bool MarbleBoard::Equals(const MarbleBoard* other) const
{
	cout << "Comparing two boards..." << std::endl;

	// Check if sizes are the same
	if (Tiles.size() != other->Tiles.size())
	{
		cout << "Tile size mismatch: " << Tiles.size() << " vs " << other->Tiles.size() << std::endl;
		return false;
	}

	// Compare each tile
	for (size_t i = 0; i < Tiles.size(); ++i)
	{
		cout << "Comparing Tile at index " << i << std::endl;
		if (!Tiles[i]->Equals(other->Tiles[i]))  // Corrected this from your code: should be if (!Equals(...))
		{
			cout << "Tiles at index " << i << " are not equal!" << std::endl;
			return false;
		}
	}

	cout << "Boards are equal!" << std::endl;
	return true;
}

void MarbleBoard::CopyBoard(const MarbleBoard* InBoard)
{
	GenerateBoard();
	for (int i = 0; i < Tiles.size(); ++i)
	{
		Tiles[i]->SetGridLocation(InBoard->Tiles[i]->GetGridLocation());
		Tiles[i]->SetSlotState(InBoard->Tiles[i]->GetSlotState());
	}
}

bool Tile::Equals(const Tile* other) const
{
	cout << "Comparing tiles at location (" << GridLocation.Column << ", " << GridLocation.Row << ")" << std::endl;

	// Check if SlotState is equal
	if (SlotState != other->GetSlotState())
	{
		cout << "SlotState mismatch: ";
		MarbleSolitare::PrintSlotState(SlotState);
		cout << " vs ";
		MarbleSolitare::PrintSlotState(other->GetSlotState());
		cout << std::endl;
		return false;
	}

	// Check if GridLocation is equal
	if (!GridLocation.Equals(other->GetGridLocation()))
	{
		cout << "GridLocation mismatch: ";
		MarbleSolitare::PrintLocation(GridLocation);
		cout << " vs ";
		MarbleSolitare::PrintLocation(other->GetGridLocation());
		cout << std::endl;
		return false;
	}

		// Compare each tile
	for (size_t i = 0; i < PossibleJumpDirections.size(); ++i)
	{
		cout << "Comparing PossibleJumpDirection at index " << i << std::endl;
		if (!PossibleJumpDirections[i] == other->PossibleJumpDirections[i])  // Corrected this from your code: should be if (!Equals(...))
		{
				cout << "Direction mismatch: ";
				MarbleSolitare::PrintDirection(PossibleJumpDirections[i]);
				cout << " vs ";
				MarbleSolitare::PrintDirection(other->PossibleJumpDirections[i]);
				cout << std::endl;
				return false;
			return false;
		}
	}
	cout << "Tiles are equal!" << std::endl;
	return true;
}

bool Location::Equals(const Location& other) const
{
	cout << "Comparing Locations: (" << Column << ", " << Row << ") vs (" << other.Column << ", " << other.Row << ")" << std::endl;
	return Column == other.Column && Row == other.Row;
}

MarbleBoard* MarbleSolitare::GetTopBoard()
{
	cout << "Fetching top board from queue..." << std::endl;
	MarbleBoard* popped_board = move(BoardQueue.front());
	BoardQueue.pop();
	VisitedBoards.push_back(popped_board);
	return popped_board;
}

MarbleBoard* MarbleBoard::SimulateMove(const Location& startLocation, const Direction& direction)
{
	cout << "Simulating move from location (" << startLocation.Column << ", " << startLocation.Row << ") in direction ";
	MarbleSolitare::PrintDirection(direction);
	cout << std::endl;

	MarbleBoard* new_board = new MarbleBoard();
	new_board->GenerateBoard();
	new_board->CopyBoard(this);

	// Find the start marble, the marble being jumped over, and the empty destination
	Tile* start_marble = new_board->FindTileByGridLocation(startLocation);
	Tile* jumped_marble = new_board->FindTileByDirection(startLocation, direction, 1);
	Tile* empty_space = new_board->FindTileByDirection(startLocation, direction, 2);

	// Check if the move is valid (i.e., there's a marble to jump over and an empty space to land on)
	if (!start_marble || !jumped_marble || !empty_space)
	{
		cout << "Invalid move: missing tile(s)" << std::endl;
		return nullptr;
	}

	if (start_marble->GetSlotState() != MarbleSlot::Marble ||
		jumped_marble->GetSlotState() != MarbleSlot::Marble ||
		empty_space->GetSlotState() != MarbleSlot::Empty)
	{
		cout << "Invalid move: slot states not suitable for a jump" << std::endl;
		return nullptr;
	}

	// Simulate the jump: set the start and jumped marble slots to empty, and the destination to a marble
	cout << "Performing jump..." << std::endl;
	start_marble->SetSlotState(MarbleSlot::Empty);
	jumped_marble->SetSlotState(MarbleSlot::Empty);
	empty_space->SetSlotState(MarbleSlot::Marble);

	return new_board;
}

void MarbleSolitare::AddBoardToQueue(MarbleBoard* InMarbleBoard)
{
	cout << "Attempting to add board to queue..." << std::endl;

	// Check if the board is already in the visited boards
	for (MarbleBoard* visited_board : VisitedBoards)
	{
		if (visited_board->Equals(InMarbleBoard))
		{
			cout << "Board is already in visited list, skipping." << std::endl;
			return;
		}
	}

	// Check if the board is already in the queue
	auto copy_queue = BoardQueue; // Make a copy of the queue
	while (!copy_queue.empty())
	{
		auto current_board = copy_queue.front(); // Access the front element
		if (current_board->Equals(InMarbleBoard))
		{
			cout << "Board is already in queue, skipping." << std::endl;
			return;
		}
		copy_queue.pop(); // Remove the processed element from the copy
	}

	cout << "Adding new board to the queue." << std::endl;
	BoardQueue.push(move(InMarbleBoard));
}
