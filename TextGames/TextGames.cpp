#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>

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

	void SaveBoard(std::string& OutFileName) 
	{
		OutFileName += "Board_";
		OutFileName += std::to_string(Index);
		OutFileName += ".txt";


		std::ifstream in_file(OutFileName);
		if (in_file.is_open())
		{
			std::stringstream buffer;
			buffer << in_file.rdbuf();  // Read the entire file content
			std::string file_content = buffer.str();
			
			if (file_content.find("File end") != std::string::npos)
			{
				return;
			}
		}

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

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
				if (BoardArray[loc] == SlotStatus::Blocked)
					out_file << "  ";  // Blocked spot
				else if (BoardArray[loc] == SlotStatus::Empty)
					out_file << "X ";  // Empty spot
				else if (BoardArray[loc] == SlotStatus::Marble)
					out_file << "O ";  // Marble present
			}
			out_file << std::endl;  // Newline after each row
		}
		

		for (const auto& move : Moves)
		{
			out_file << "Starting Board Index: " << move.BoardIndex
				<< " Location: " << move.StartingLocation.ToString()
				<< " Direction: " << DirectionToString(move.MoveDirection) << std::endl;
		}


		out_file<<"Board" << std::endl;

		for (std::pair<Location, SlotStatus> marble_pair : BoardArray)
		{
			out_file << "{" << std::endl;
			out_file <<  marble_pair.first.ToString() << ";" << std::endl;
			out_file <<  StatusToString(marble_pair.second) << ";" << std::endl;
			out_file << "}" << std::endl;
		}
		out_file << "~Board" << std::endl;

		out_file << "Moves" << std::endl;
		for (const auto& move : Moves)
		{
			out_file << "{";
				out_file << std::to_string(move.BoardIndex) << ";" << std::endl;
				out_file << move.StartingLocation.ToString() << ";" << std::endl;
				out_file << DirectionToString(move.MoveDirection) << ";" << std::endl;
				out_file << "}" << std::endl;
		}
		out_file <<  "~Moves" << std::endl;

		out_file <<  "Index" << std::endl;
		out_file << std::to_string(Index) << std::endl;
		out_file << "~Index" << std::endl;

		out_file << "NextIndex" << std::endl;
		out_file << std::to_string(Index) << std::endl;
		out_file << "~NextIndex" << std::endl;

		out_file <<  "MarbleCount" << std::endl;
		out_file << std::to_string(MarbleCount) << std::endl;
		out_file << "~MarbleCount" << std::endl;
		out_file << "File end" << std::endl;
	}

	Board(const std::string& InFileName)
	{
		std::ifstream saved_board_file(InFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Board loaded from " << InFileName << std::endl;
		}
		else 
		{
			std::cerr << "Unable to open file for loading: " << InFileName << std::endl;
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end) 
			{
			return InBoardSave.substr(InBoardSave.find(start) + start.length(),
				InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string board_str = extract_substring("Board", "~Board");
		std::string moves_str = extract_substring("Moves", "~Moves");
		std::string index_str = extract_substring("Index", "~Index");
		std::string marble_str = extract_substring("MarbleCount", "~MarbleCount");

		// Rebuild the board
		BoardArray.clear();
		{
			std::pair<Location, SlotStatus> marble_pair;
			std::string storage_string;
			int fill_target = 0;

			for (char current_character : board_str)
			{
				if (current_character == '\n')
					continue;
				if (current_character == '{') continue;
				if (current_character == '}')
				{
					BoardArray[marble_pair.first] = marble_pair.second;
					storage_string.clear();
					marble_pair = {};
					fill_target = 0;
					continue;
				}
				if (current_character == ';')
				{
					if (fill_target == 0)
						marble_pair.first = Location(storage_string);
					else if (fill_target == 1)
						marble_pair.second = StringToStatus(storage_string);
					fill_target++;
					storage_string.clear();
					continue;
				}
				storage_string += current_character;
			}
		}

		// Rebuild the moves
		{
			Move created_move;
			std::string storage_string;
			int fill_target = 0;

			for (char current_character : moves_str)
			{
				if (current_character == '\n')
					continue;
				if (current_character == '{') continue;
				if (current_character == '}')
				{
					Moves.push_back(created_move);
					storage_string.clear();
					created_move = Move();
					fill_target = 0;
					continue;
				}
				if (current_character == ';')
				{
					if (fill_target == 0)
						created_move.BoardIndex = std::stoi(storage_string);
					else if (fill_target == 1)
						created_move.StartingLocation = Location(storage_string);
					else if (fill_target == 2)
						created_move.MoveDirection = StringToDirection(storage_string);
					fill_target++;
					storage_string.clear();
					continue;
				}
				storage_string += current_character;
			}
		}

		// Parse index and marble count
		Index = std::stoi(index_str);
		MarbleCount = std::stoi(marble_str);
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
	std::vector<std::string> WinningBoards;
	std::vector<std::string> BoardQueue;
	std::unordered_map<std::string, Board*> ExploredBoards;
	std::map<int, int> MarbleIterations;
	
	void Simulate(bool DFS)
	{
		std::string file_name;
		Board().SaveBoard(file_name);
		BoardQueue.push_back(file_name);
		LoadWinningBoards();
		LoadIterations();
		LoadQueue();
		
		while (BoardQueue.empty() == false)
		{
			Board* popped_board ;
			if (!DFS)
			{
				popped_board = new Board(BoardQueue.front());
				BoardQueue.erase(BoardQueue.begin());
			}
			else
			{
				popped_board = new Board(BoardQueue.back());
				BoardQueue.erase(BoardQueue.end() - 1);    
			}

			ExploreMoves(popped_board, false);
			SaveWinningBoards();
			SaveIterations();
			SaveQueue();
		}
	}

	void LoadWinningBoards()
	{

	}

	void LoadIterations()
	{

	}
	void LoadQueue()
	{

	}
	void SaveWinningBoards()
	{

	}

	void SaveIterations()
	{

	}
	void SaveQueue()
	{

	}

	void ExploreMoves(Board* InCurrentBoard, bool DFS)
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
			if (DFS)
				ExploredBoards[InCurrentBoard->Hash] = InCurrentBoard;
			WinningBoards.push_back(InCurrentBoard->SaveBoard();
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
				std::string file_name;
				copied_board->SaveBoard(file_name);
				BoardQueue.push_back(file_name);
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

	solitare.Simulate(false);
	//solitare.SimulatedDFS();

	solitare.PrintWinningBoards();

	return 0;  // Return 0 to indicate successful execution
}