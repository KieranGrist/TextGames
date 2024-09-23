#include <iostream>
#include <map>
#include <vector>
#include <string>

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

// Loction struct
struct Location
{
	Location() {}

	Location(int InX, int InY) : X(InX), Y(InY) {}

	std::string ToString() const
	{
		return "Location: " + std::to_string(X) + ", " + std::to_string(Y);
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

	int BoardIndex;
	Location StartingLocation;
	Direction MoveDirection;
};


class Board
{
public:
	int Index = 0;
	bool Explored = false;
	static int NextIndex;
	// Define the standard English Marble Solitaire board (7x7 grid) using std::map
	std::map<Location, SlotStatus> BoardArray;
	std::vector<Move> Moves;

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
	}

	// Copy constructor
	Board(const Board* InOther)
	{
		BoardArray = InOther->BoardArray;
		Moves = InOther->Moves;
		Index = InOther->Index;
	}
	void UpdateIndex()
	{
		NextIndex++;
		Index = NextIndex;
	}

	std::string DirectionToString(Direction InDirection) const
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

	std::string StatusToString(SlotStatus InStatus) const
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

	int CountMarbles() const
	{
		int marble_count = 0;
		for (auto marble_pair : BoardArray)
			if (marble_pair.second == SlotStatus::Marble)
				marble_count++;
		return marble_count;
	}

	// Function to check if the board is in a winning state (1 marble left in the center)
	bool IsWinningState()
	{
		return CountMarbles() == 1;
	}


};

class SolitareSimulation
{
public:
	std::vector<Board*> WinningBoards;
	std::vector<Board*> BoardQueue;
	std::vector<Board*> CreatedBoards;

	static void ProcessMove(const Move& InMove, Location& OutStartLocation, Location& OutMarbleLocation, Location& OutEmptyLocation)
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

	Board* FindOrCreateBoard(const Board* InOriginalBoard, const Move& InMove)
	{
	
		Location start_location;
		Location marble_location;
		Location empty_location;
		ProcessMove(InMove, start_location, marble_location, empty_location);
		
		Board search_board = Board(InOriginalBoard);
		search_board.BoardArray[start_location] = SlotStatus::Empty;
		search_board.BoardArray[marble_location] = SlotStatus::Empty;
		search_board.BoardArray[empty_location] = SlotStatus::Marble;
		auto predicate = [search_board](Board* current_board) 
		{
			return search_board.BoardArray == current_board->BoardArray;
		};

		auto it = std::find_if(CreatedBoards.begin(), CreatedBoards.end(), predicate);
		if (it != CreatedBoards.end())
			return *it;
	
		Board* out_board = new Board(search_board);
		out_board->UpdateIndex();

		CreatedBoards.push_back(out_board);
		return out_board;
	}

	// Function to make a move
	Board* Jump(const Board* InBoard, const Move& InMove)
	{
		//std::cout << "Attempting move from " << InLocation.ToString() << " in direction " << DirectionToString(InDirection) << std::endl;
		Location start_location;
		Location marble_location;
		Location empty_location;
		ProcessMove(InMove, start_location, marble_location, empty_location);

		// Find the locations in the board
		auto starting_marble = InBoard->BoardArray.find(start_location);
		auto jump_marble = InBoard->BoardArray.find(marble_location);
		auto empty_slot = InBoard->BoardArray.find(empty_location);

		// Check if the locations exist and are valid
		if (starting_marble == InBoard->BoardArray.end() || jump_marble == InBoard->BoardArray.end() || empty_slot == InBoard->BoardArray.end())
			return nullptr;

		if (starting_marble->second != SlotStatus::Marble || jump_marble->second != SlotStatus::Marble || empty_slot->second != SlotStatus::Empty)
			return nullptr;

		return FindOrCreateBoard(InBoard, InMove);
	}

	void SimulateBFS()
	{
		Board* FirstBoard = new Board();
		CreatedBoards.push_back(FirstBoard);
		BoardQueue.push_back(FirstBoard);
		while (BoardQueue.empty() == false)
		{
			Board* popped_board = BoardQueue.front();
			BoardQueue.erase(BoardQueue.begin());
			ExploreMoves(popped_board);
		}
	}

	void SimulatedDFS()
	{
		Board* FirstBoard = new Board();
		CreatedBoards.push_back(FirstBoard);
		BoardQueue.push_back(FirstBoard);
		while (BoardQueue.empty() == false)
		{
			Board* popped_board = BoardQueue.back(); // Get the top board
			BoardQueue.pop_back();
			ExploreMoves(popped_board);
		}
	}


	void ExploreMoves(Board* InCurrentBoard)
	{
		if (InCurrentBoard->Explored)
			return;
		InCurrentBoard->Explored = true;
		std::cout << "------------------" << std::endl;
		std::cout << "Exploring moves for Board Index " << InCurrentBoard->Index << std::endl;
		//CurrentBoard.PrintBoard();

		// Base case: check if it's a winning state
		if (InCurrentBoard->IsWinningState())
		{
			WinningBoards.push_back(InCurrentBoard);
			std::cout << "Winning state found! back tracking" << std::endl;
			std::cout << "------------------" << std::endl;
			return;
		}

		for (auto marble_pair : InCurrentBoard->BoardArray)
		{
			if (marble_pair.second != SlotStatus::Marble)
				continue;
			for (auto direction : { Direction::Up,	Direction::Right, Direction::Down, Direction::Left })
			{
				Move next_move(InCurrentBoard->Index, marble_pair.first, direction);
				if (Board* copied_board = Jump(InCurrentBoard, next_move))
				{
					copied_board->Moves.push_back(next_move);
					std::cout << std::endl;
					std::cout << "Board: " << InCurrentBoard->Index << " Has " << InCurrentBoard->CountMarbles() << " Marbles Left" << std::endl;
					InCurrentBoard->PrintBoard();
					std::cout << "Move made from " << marble_pair.first.ToString() << " in direction " << copied_board->DirectionToString(direction) << std::endl;
					std::cout << "Board: " << copied_board->Index << " Has " << copied_board->CountMarbles() << " Marbles Left" << std::endl;
					copied_board->PrintBoard();
					if (copied_board->Explored == false)
						BoardQueue.push_back(copied_board);
					//ExploreMoves(copied_board);	
				}
			}
		}
		std::cout << "------------------" << std::endl;
		std::cout << std::endl << std::endl;
	}
};

int Board::NextIndex = 0; // Start indexing from 0

int main()
{
	SolitareSimulation solitare;

	solitare.SimulatedDFS();

	return 0;  // Return 0 to indicate successful execution
}