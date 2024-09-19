#include "MarbleSolitare.h"
#include "Tile.h"
#include "MarbleBoard.h"

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

vector<unique_ptr<MarbleBoard>> MarbleSolitare::SimulateMove(const vector<unique_ptr<MarbleBoard>> InMarbleBoard, const Location& InStartLocation, const Direction& InDirection)
{
	//MarbleSolitare::PrintLocation(InStartLocation);
	//cout << " Jumps in direction ";
	//MarbleSolitare::PrintDirection(InDirection);
	//cout << endl;

	vector<unique_ptr<MarbleBoard>> new_board = new MarbleBoard();
	new_board->CopyBoard(InMarbleBoard);

	// Find the start marble, the marble being jumped over, and the empty destination
	Tile* start_marble = new_board->FindTileByGridLocation(InStartLocation);
	Tile* jumped_marble = new_board->FindTileByDirection(InStartLocation, InDirection, 1);
	Tile* empty_space = new_board->FindTileByDirection(InStartLocation, InDirection, 2);

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
		//	cout << "Invalid move: slot states not suitable for a jump" << std::endl;
		return nullptr;
	}

	// Simulate the jump: set the start and jumped marble slots to empty, and the destination to a marble
	//cout << "Performing jump..." << std::endl;
	start_marble->SetSlotState(MarbleSlot::Empty);
	jumped_marble->SetSlotState(MarbleSlot::Empty);
	empty_space->SetSlotState(MarbleSlot::Marble);

	return new_board;
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

void MarbleSolitare::MarbleSolitare::PrintDirection(const Direction& InDirection)
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

void MarbleSolitare::MarbleSolitare::PrintSlotState(const MarbleSlot& InMarbleSlot)
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

void MarbleSolitare::MarbleSolitare::PrintLocation(const Location& InLocation)
{
	cout << "Location:" << InLocation.Column << InLocation.Row;
}

void MarbleSolitare::MarbleSolitare::PrintTile(const Tile& InTitle)
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

void MarbleSolitare::MarbleSolitare::PrintBoard(const MarbleBoard& InMarbleBoard)
{
	// MarbleSolitare::Print the board with row numbers
	for (auto tile : InMarbleBoard.GetTiles())
	{
		MarbleSolitare::PrintTile(*tile);
	}
	cout << endl;
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
	vector<unique_ptr<MarbleBoard>> start_board = new MarbleBoard();
	start_board->GenerateBoard();
	start_board->GenerateBoardPossibleJumpDirections();

	//queue<vector<unique_ptr<MarbleBoard>>> board_queue;  // Queue for BFS
	AddBoardToQueue(start_board);
	int wins = 0;
	int boards = 0;
	int jumps = 0;
	while (!BoardQueue.empty())
	{
		vector<unique_ptr<MarbleBoard>> current_board = GetTopBoard();
		boards++;
		//MarbleSolitare::PrintBoard(*current_board);
		// Collect statistics for the current board state
		int marbleCount = current_board->MarbleCount();
		cout << "Number of marbles remaining: " << marbleCount << endl;
		cout << "Boards: " << boards << " Board left: " << BoardQueue.size() << endl;
		cout << "Wins: " << wins << " Boards: " << boards << " Jumps " << jumps << endl;

		//	current_board->MarbleSolitare::PrintValidSelections();

		// Generate possible moves from the current board state

		auto moveable_marbles = current_board->GetMoveableMarbles();
		for (Tile* tile : moveable_marbles)
		{
			for (const Direction& direction : tile->GetPossibleJumpDirections())
			{
				// Simulate the move and add the new board state to the queue
				AddBoardToQueue(MarbleSolitare::SimulateMove(current_board, tile->GetGridLocation(), direction));
				jumps++;
			}
		}

		if (current_board->MarbleCount() == 1)
			wins++;
	}
	cout << "Wins: " << wins << " Boards: " << boards << " Jumps " << jumps;
}

void MarbleSolitare::AddBoardToQueue(vector<unique_ptr<MarbleBoard>> InMarbleBoard)
{
	//cout << "Attempting to add board to queue..." << std::endl;

		// Check if the board is already in the visited boards
	for (vector<unique_ptr<MarbleBoard>> visited_board : VisitedBoards)
	{
		if (visited_board->Equals(InMarbleBoard))
		{
			//cout << "Board is already in visited list, skipping." << std::endl;
			return;
		}
	}

	for (vector<unique_ptr<MarbleBoard>> visited_board : BoardQueue)
	{
		if (visited_board->Equals(InMarbleBoard))
		{
			//cout << "Board is already in visited list, skipping." << std::endl;
			return;
		}
	}

	BoardQueue.push_back(move(InMarbleBoard));
	cout << "Added Board to queue. Boards left: " << BoardQueue.size() << endl;
}

vector<unique_ptr<MarbleBoard>> MarbleSolitare::GetTopBoard()
{
	vector<unique_ptr<MarbleBoard>> popped_board = BoardQueue.front();
	BoardQueue.erase(BoardQueue.begin());
	VisitedBoards.push_back(popped_board);
	popped_board->GenerateBoardPossibleJumpDirections();
	return popped_board;
}
