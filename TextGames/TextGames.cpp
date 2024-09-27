#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

enum class Direction
{
	Error,
	Up,
	Right,
	Down,
	Left
};

enum class SlotStatus
{
	Blocked,
	Empty,
	Marble
};


std::string DirectionToString(Direction InDirection)
{
	switch (InDirection)
	{
	case Direction::Error:
		break;
	case Direction::Up:
		return "Up";
		break;
	case Direction::Right:
		return "Right";
		break;
	case Direction::Down:
		return "Down";
		break;
	case Direction::Left:
		return "Left";
		break;
	default:
		break;
	}
	return "";
}

std::string StatusToString(SlotStatus InStatus)
{
	switch (InStatus)
	{
	case SlotStatus::Blocked:
		return "Blocked";
		break;
	case SlotStatus::Empty:
		return "Empty";
		break;
	case SlotStatus::Marble:
		return "Marble";
		break;
	default:
		break;
	}
	return "";
}

// Loction struct
struct Location
{
	Location() {}

	Location(int InX, int InY) : X(InX), Y(InY) {}

	std::string ToString() const
	{
		return  std::to_string(X) + ", " + std::to_string(Y);
	}

	// Comparison operators for map keys
	bool operator==(const Location& InOther) const
	{
		return X == InOther.X && Y == InOther.Y;
	}

	bool operator<(const Location& InOther) const
	{
		return std::tie(X, Y) < std::tie(InOther.X, InOther.Y);
	}

	int X = -1;
	int Y = -1;
};

// Loction
struct Move
{
	Move(int InBoardIndex,
		Location InStartingLocation,
		Direction InMoveDirection) :
		BoardIndex(InBoardIndex),
		StartingLocation(InStartingLocation),
		MoveDirection(InMoveDirection)
	{}
	void PrintMove() const
	{
		std::cout << "Starting Board Index " << BoardIndex << " Location: " << StartingLocation.ToString()
			<< " Direction: " << DirectionToString(MoveDirection);
	}

	std::string HashMove() const
	{
		return  std::to_string(BoardIndex) + " " + StartingLocation.ToString()
			+ " " + DirectionToString(MoveDirection);
	}

	int BoardIndex;
	Location StartingLocation;
	Direction MoveDirection;
};

class Board
{
public:
	int Index = 0;
	int MarbleCount = 0;
	static int NextIndex;
	// Define the standard English Marble Solitaire board (7x7 grid) using std::map
	std::map<Location, SlotStatus> BoardArray;
	std::vector<Move> Moves;
	std::string Hash;

	~Board()
	{
		Moves.clear();
		BoardArray.clear();
	}
	Board()
	{
		BoardArray =
		{
			{ Location(0, 0), SlotStatus::Blocked }, { Location(1, 0), SlotStatus::Blocked }, { Location(2, 0), SlotStatus::Marble}, { Location(3, 0), SlotStatus::Marble}, { Location(4, 0), SlotStatus::Marble}, { Location(5, 0), SlotStatus::Blocked }, { Location(6, 0), SlotStatus::Blocked },
			{ Location(0, 1), SlotStatus::Blocked }, { Location(1, 1), SlotStatus::Blocked }, { Location(2, 1), SlotStatus::Marble}, { Location(3, 1), SlotStatus::Marble}, { Location(4, 1), SlotStatus::Marble}, { Location(5, 1), SlotStatus::Blocked }, { Location(6, 1), SlotStatus::Blocked },
			{ Location(0, 2), SlotStatus::Marble}, { Location(1, 2), SlotStatus::Marble}, { Location(2, 2), SlotStatus::Marble}, { Location(3, 2), SlotStatus::Marble}, { Location(4, 2), SlotStatus::Marble}, { Location(5, 2), SlotStatus::Marble}, { Location(6, 2), SlotStatus::Marble},
			{ Location(0, 3), SlotStatus::Marble}, { Location(1, 3), SlotStatus::Marble}, { Location(2, 3), SlotStatus::Marble}, { Location(3, 3),  SlotStatus::Empty }, { Location(4, 3), SlotStatus::Marble}, { Location(5, 3), SlotStatus::Marble}, { Location(6, 3), SlotStatus::Marble},
			{ Location(0, 4), SlotStatus::Marble}, { Location(1, 4), SlotStatus::Marble}, { Location(2, 4), SlotStatus::Marble}, { Location(3, 4), SlotStatus::Marble}, { Location(4, 4), SlotStatus::Marble}, { Location(5, 4), SlotStatus::Marble}, { Location(6, 4), SlotStatus::Marble},
			{ Location(0, 5), SlotStatus::Blocked }, { Location(1, 5), SlotStatus::Blocked }, { Location(2, 5), SlotStatus::Marble}, { Location(3, 5), SlotStatus::Marble}, { Location(4, 5), SlotStatus::Marble}, { Location(5, 5), SlotStatus::Blocked }, { Location(6, 5), SlotStatus::Blocked },
			{ Location(0, 6), SlotStatus::Blocked }, { Location(1, 6), SlotStatus::Blocked }, { Location(2, 6), SlotStatus::Marble}, { Location(3, 6), SlotStatus::Marble}, { Location(4, 6), SlotStatus::Marble}, { Location(5, 6), SlotStatus::Blocked }, { Location(6, 6), SlotStatus::Blocked }
		};
		HashBoard();
	}

	// Copy constructor
	Board(const Board* InOther)
	{
		BoardArray = InOther->BoardArray;
		Moves = InOther->Moves;
		Index = InOther->Index;
		HashBoard();
	}

	// Copy constructor
	Board(const Board& InOther)
	{
		BoardArray = InOther.BoardArray;
		Moves = InOther.Moves;
		Index = InOther.Index;
		HashBoard();
	}
	void UpdateIndex()
	{
		NextIndex++;
		Index = NextIndex;
	}

	void PrintBoard()
	{
		// Loop through each Y coordinate (row) from 0 to 6 (7x7 grid)
		std::cout << "  ";
		for (int y = 0; y < 7; ++y)
			std::cout << y << " ";
		std::cout << std::endl;
		for (int y = 0; y < 7; ++y)
		{
			std::cout << y << " ";
			// Loop through each X coordinate (column) from 0 to 6
			for (int x = 0; x < 7; ++x)
			{
				Location loc(x, y); // Create the location to check in the BoardArray

				// Check the status of each slot
				if (BoardArray[loc] == SlotStatus::Blocked)
					std::cout << "  ";  // Blocked spot
				else if (BoardArray[loc] == SlotStatus::Empty)
					std::cout << "X ";  // Empty spot
				else if (BoardArray[loc] == SlotStatus::Marble)
					std::cout << "O ";  // Marble present
			}
			std::cout << std::endl;  // Newline after each row
		}
	}

	void HashBoard()
	{
		Hash ="";
		for (auto marble_pair : BoardArray)
		{
			Hash += marble_pair.first.ToString() + StatusToString(marble_pair.second) + " ";
		}
	}

	int UpdateMarbleCount()
	{
		MarbleCount = 0;
		for (auto marble_pair : BoardArray)
			if (marble_pair.second == SlotStatus::Marble)
				MarbleCount++;
		return MarbleCount;
	}

	// Function to check if the board is in a winning state (1 marble left in the center)
	bool IsWinningState()
	{
		return UpdateMarbleCount() == 1;
	}

	bool HasNoValidMoves() const
	{
		for (auto marble_pair : BoardArray)
		{
			if (marble_pair.second != SlotStatus::Marble)
				continue;
			for (auto direction : { Direction::Up,	Direction::Right, Direction::Down, Direction::Left })
			{
				Move next_move(Index, marble_pair.first, direction);
				if (IsMoveValid(next_move))
					return false;
			}
		}
		return true;
	}

	void GetLocationsFromMove(const Move& InMove, Location& OutStartLocation, Location& OutMarbleLocation, Location& OutEmptyLocation) const
	{
		OutStartLocation = InMove.StartingLocation;
		OutMarbleLocation = InMove.StartingLocation;
		OutEmptyLocation = InMove.StartingLocation;

		// Adjust coordinates based on the direction
		switch (InMove.MoveDirection)
		{
		case Direction::Error:
			return;
		case Direction::Up:
			OutMarbleLocation.Y -= 1;
			OutEmptyLocation.Y -= 2;
			break;
		case Direction::Right:
			OutMarbleLocation.X += 1;
			OutEmptyLocation.X += 2;
			break;
		case Direction::Down:
			OutMarbleLocation.Y += 1;
			OutEmptyLocation.Y += 2;
			break;
		case Direction::Left:
			OutMarbleLocation.X -= 1;
			OutEmptyLocation.X -= 2;
			break;
		}
	}

	bool IsMoveValid(const Move& InMove) const
	{
		Location start_location;
		Location marble_location;
		Location empty_location;
		GetLocationsFromMove(InMove, start_location, marble_location, empty_location);
		// Find the locations in the board
		auto starting_marble = BoardArray.find(start_location);
		auto jump_marble = BoardArray.find(marble_location);
		auto empty_slot = BoardArray.find(empty_location);

		// Check if the locations exist and are valid
		if (starting_marble == BoardArray.end() || jump_marble == BoardArray.end() || empty_slot == BoardArray.end())
			return false;

		if (starting_marble->second != SlotStatus::Marble || jump_marble->second != SlotStatus::Marble || empty_slot->second != SlotStatus::Empty)
			return false;
		return true;
	}

	// Function to make a move
	void Jump(const Move& InMove)
	{
		if (!IsMoveValid(InMove))
			return;
		Location start_location;
		Location marble_location;
		Location empty_location;
		GetLocationsFromMove(InMove, start_location, marble_location, empty_location);
		BoardArray[start_location] = SlotStatus::Empty;
		BoardArray[marble_location] = SlotStatus::Empty;
		BoardArray[empty_location] = SlotStatus::Marble;
		HashBoard();
	}
};

class SolitareSimulation
{
public:
	std::vector<Board*> WinningBoards;
	std::vector<Board*> BoardQueue;
	std::unordered_map<std::string, Board*> ExploredBoards;
	std::map<int, int> MarbleIterations;
	void SimulateBFS()
	{
		BoardQueue.push_back(new Board());
		while (BoardQueue.empty() == false)
		{
			Board* popped_board = BoardQueue.front();
			BoardQueue.erase(BoardQueue.begin());
			ExploreMoves(popped_board, false);
		}
	}

	void SimulatedDFS()
	{
		BoardQueue.push_back(new Board());
		while (BoardQueue.empty() == false)
		{
			Board* popped_board = BoardQueue.back(); // Get the top board
			BoardQueue.pop_back();
			ExploreMoves(popped_board, true);
		}
	}

	void ExploreMoves(Board* InCurrentBoard, bool DFS)
	{
		MarbleIterations[InCurrentBoard->UpdateMarbleCount()]++;
		std::cout << "Exploring moves for Board Index " << InCurrentBoard->Index << " Marbles left: " << InCurrentBoard->UpdateMarbleCount()
			<< " Iterations for marble amount "
			<< MarbleIterations[InCurrentBoard->UpdateMarbleCount()]
			<< std::endl;

		//CurrentBoard.PrintBoard();

		// Base case: check if it's a winning state
		if (InCurrentBoard->IsWinningState())
		{
			if (DFS)
				ExploredBoards[InCurrentBoard->Hash] = InCurrentBoard;
			WinningBoards.push_back(InCurrentBoard);
			return;
		}
		if (InCurrentBoard->HasNoValidMoves())
		{
			if (DFS)
				ExploredBoards[InCurrentBoard->Hash] = InCurrentBoard;
			return;
		}

		for (auto marble_pair : InCurrentBoard->BoardArray)
		{
			if (marble_pair.second != SlotStatus::Marble)
				continue;
			for (auto direction : { Direction::Up,	Direction::Right, Direction::Down, Direction::Left })
			{
				Move next_move(InCurrentBoard->Index, marble_pair.first, direction);
				if (!InCurrentBoard->IsMoveValid(next_move))
					continue;

				Board* copied_board = new Board(InCurrentBoard);
				copied_board->Jump(next_move);
				if (DFS && ExploredBoards.find(copied_board->Hash) != ExploredBoards.end())
					continue;

				copied_board->Moves.push_back(next_move);
				copied_board->UpdateIndex();
				//ExploreMoves(copied_board);
				BoardQueue.push_back(copied_board);
			}
		}
		if (DFS)
			ExploredBoards[InCurrentBoard->Hash] = InCurrentBoard;
	}

	void PrintWinningBoards()
	{
		for (auto board : WinningBoards)
		{
			Board display_board;
			for (const Move& move : board->Moves)
			{
				display_board.Jump(move);
				std::cout << "Move: ";
				move.PrintMove();
				display_board.PrintBoard();
			}
		}
	}
};

int Board::NextIndex = 0; // Start indexing from 0

int main()
{
	SolitareSimulation solitare;

	solitare.SimulateBFS();
	//solitare.SimulatedDFS();

	solitare.PrintWinningBoards();

	return 0;  // Return 0 to indicate successful execution
}