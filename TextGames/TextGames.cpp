#include <algorithm> // For std::sort
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>  // For std::atexit
#include <iostream>
#include <chrono>
#include <thread>

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
	Error,
	Blocked,
	Empty,
	Marble
};

template <typename T>
class TArray : public std::vector<T>
{
public:
	// Default constructor
	TArray() : std::vector<T>() {}

	// Constructor that forwards arguments to std::vector's constructor
	TArray(std::initializer_list<T> init) : std::vector<T>(init) {}

	// Checks if a value is contained within the array
	bool Contains(const T& value) const
	{
		// Use this-> to access inherited member functions
		return std::find(this->begin(), this->end(), value) != this->end();
	}
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

SlotStatus StringToStatus(const std::string& InString)
{
	if (InString == "Blocked")
		return SlotStatus::Blocked;
	if (InString == "Empty")
		return SlotStatus::Empty;
	if (InString == "Marble")
		return SlotStatus::Marble;
	return SlotStatus::Error;
}

Direction StringToDirection(const std::string& InString)
{
	if (InString == "Up")
		return Direction::Up;
	if (InString == "Right")
		return Direction::Right;
	if (InString == "Down")
		return Direction::Down;
	if (InString == "Left")
		return Direction::Left;
	return Direction::Error;
}

int NextIndex = 0;

// Loction struct
struct Location
{
	Location() {}

	Location(int InX, int InY) : X(InX), Y(InY) {}

	// Constructor that takes a string in the format "X,Y"
	Location(const std::string& InString)
	{
		// Find the position of the comma
		size_t comma_pos = InString.find(',');

		// Extract the substring before the comma (X value)
		X = std::stoi(InString.substr(0, comma_pos));

		// Extract the substring after the comma (Y value)
		Y = std::stoi(InString.substr(comma_pos + 1));
	}

	std::string ToString() const
	{
		return std::to_string(X) + "," + std::to_string(Y);
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
	Move()
	{
		BoardIndex = -1;
		StartingLocation = Location();
		MoveDirection = Direction::Error;
	}

	Move(int InBoardIndex,
		Location InStartingLocation,
		Direction InMoveDirection) :
		BoardIndex(InBoardIndex),
		StartingLocation(InStartingLocation),
		MoveDirection(InMoveDirection)
	{

	}
	void PrintMove() const
	{
		std::cout << "Starting Board Index: " << BoardIndex
			<< " Location: " << StartingLocation.ToString()
			<< " Direction: " << DirectionToString(MoveDirection);
	}

	int BoardIndex;
	Location StartingLocation;
	Direction MoveDirection;
};

class Board
{
public:
	int Index = 0;
	int MarbleCount = 32;

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
		MarbleCount = InOther->MarbleCount;
		Index = InOther->Index;
		HashBoard();
	}

	// Copy constructor
	Board(const Board& InOther)
	{
		MarbleCount = InOther.MarbleCount;
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
		Hash = "";
		for (std::pair<Location, SlotStatus> marble_pair : BoardArray)
		{
			Hash += "{";
			Hash += marble_pair.first.ToString() + ";";
			Hash += StatusToString(marble_pair.second) + ";";
			Hash += "}";
		}
	}

	// Function to check if the board is in a winning state (1 marble left in the center)
	bool IsWinningState()
	{
		return MarbleCount == 1;
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
		MarbleCount--;
	}
};

class SolitareSimulation
{
public:
	std::map<int, int> MarbleIterations;

	TArray<Board*> BoardQueue;
	TArray<Board*> WinningBoards;
	TArray<std::string> ExploredBoards;

	bool IsBreadthFirstSearch = false;

	void AddBoardToQueue(Board* InBoard)
	{
		BoardQueue.push_back(InBoard);
	}

	Board* GetBoardFromQueue()
	{
		Board* board;
		if (IsBreadthFirstSearch)
		{
			board = BoardQueue.front();
			BoardQueue.erase(BoardQueue.begin());
		}
		else
		{
			 board = BoardQueue.back();
			BoardQueue.erase(BoardQueue.end() -1);
		}
		return board;
	}

	void AddWinningBoard(Board* InBoard)
	{
		WinningBoards.push_back(InBoard);
		SaveWinningBoards();
	}

	void AddExploredBoard(Board* InBoard)
	{
		ExploredBoards.push_back(InBoard->Hash);
	}

	void Simulate()
	{
		int iterations = 0;
		AddBoardToQueue(new Board());
		Board* popped_board = GetBoardFromQueue();
		while (popped_board)
		{
			ExploreMoves(popped_board);
			popped_board = GetBoardFromQueue();
		}
		SaveWinningBoards();
	}

	void ExploreMoves(Board* InCurrentBoard)
	{
		MarbleIterations[InCurrentBoard->MarbleCount]++;
		std::cout << "Exploring moves for Board Index " << InCurrentBoard->Index << " Marbles left: " << InCurrentBoard->MarbleCount
			<< " Iterations for marble amount "
			<< MarbleIterations[InCurrentBoard->MarbleCount]
			<< std::endl;

		//CurrentBoard.PrintBoard();

		// Base case: check if it's a winning state
		if (InCurrentBoard->IsWinningState())
		{
			if (!IsBreadthFirstSearch)
				AddExploredBoard(InCurrentBoard);
			AddWinningBoard(InCurrentBoard);
			return;
		}
		if (InCurrentBoard->HasNoValidMoves())
		{
			if (IsBreadthFirstSearch)
				AddExploredBoard(InCurrentBoard);
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
				if (!IsBreadthFirstSearch && ExploredBoards.Contains(copied_board->Hash))
					continue;

				copied_board->Moves.push_back(next_move);
				copied_board->UpdateIndex();
				//ExploreMoves(copied_board);
				AddBoardToQueue(copied_board);
			}
		}
		if (!IsBreadthFirstSearch)
			AddExploredBoard(InCurrentBoard);
	}

	void SaveWinningBoards()
	{
		std::string OutFileName;
		OutFileName += "WinningBoards";
		OutFileName += ".txt";

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

		for (Board* winning_board : WinningBoards)
		{
			out_file << winning_board->Index << std::endl;

			Board display_board;
			for (const Move& move : winning_board->Moves)
			{
				display_board.Jump(move);
				// Loop through each Y coordinate (row) from 0 to 6 (7x7 grid)
				out_file << "  ";
				for (int y = 0; y < 7; ++y)
					out_file << y << " ";
				out_file << std::endl;
				for (int y = 0; y < 7; ++y)
				{
					out_file << y << " ";
					// Loop through each X coordinate (column) from 0 to 6
					for (int x = 0; x < 7; ++x)
					{
						Location loc(x, y); // Create the location to check in the BoardArray

						// Check the status of each slot
						if (winning_board->BoardArray[loc] == SlotStatus::Blocked)
							out_file << "  ";  // Blocked spot
						else if (winning_board->BoardArray[loc] == SlotStatus::Empty)
							out_file << "X ";  // Empty spot
						else if (winning_board->BoardArray[loc] == SlotStatus::Marble)
							out_file << "O ";  // Marble present
					}
					out_file << std::endl;  // Newline after each row
				}

				for (const auto& move : winning_board->Moves)
				{
					out_file << "Starting Board Index: " << move.BoardIndex
						<< " Location: " << move.StartingLocation.ToString()
						<< " Direction: " << DirectionToString(move.MoveDirection) << std::endl;
				}
			}
		}
	}
};

int main()
{
	SolitareSimulation* solitare = new SolitareSimulation();

	solitare->IsBreadthFirstSearch = true;
	solitare->Simulate();
	//solitare.SimulatedDFS();


	return 0;  // Return 0 to indicate successful execution
}