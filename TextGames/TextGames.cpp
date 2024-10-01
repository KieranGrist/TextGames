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

	const std::string SaveBoard()
	{   // Create a folder named "Boards" if it doesn't exist
		std::string folderName = "Boards";
		std::filesystem::create_directory(folderName);

		// Construct the output file name with the folder path
		std::string OutFileName;
		OutFileName += folderName + "/";  // Add folder name
		OutFileName += "Board_";           // Prefix
		OutFileName += std::to_string(Index); // Index
		OutFileName += ".txt";

		std::ifstream in_file(OutFileName);
		if (in_file.is_open())
		{
			std::stringstream buffer;
			buffer << in_file.rdbuf();  // Read the entire file content
			std::string file_content = buffer.str();

			if (file_content.find("File end") != std::string::npos)
			{
				return OutFileName;
			}
		}

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return OutFileName;

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

		out_file << "!Board" << std::endl;
		for (std::pair<Location, SlotStatus> marble_pair : BoardArray)
		{
			out_file << "{";
			out_file << marble_pair.first.ToString() << ";";
			out_file << StatusToString(marble_pair.second) << ";";
			out_file << "}" << std::endl;
		}
		out_file << "~Board" << std::endl;

		out_file << "!Moves" << std::endl;
		for (const auto& move : Moves)
		{
			out_file << "{";
			out_file << std::to_string(move.BoardIndex) << ";";
			out_file << move.StartingLocation.ToString() << ";";
			out_file << DirectionToString(move.MoveDirection) << ";";
			out_file << "}" << std::endl;
		}
		out_file << "~Moves" << std::endl;

		out_file << "!Index" << std::endl;
		out_file << std::to_string(Index) << std::endl;
		out_file << "~Index" << std::endl;

		out_file << "!MarbleCount" << std::endl;
		out_file << std::to_string(MarbleCount) << std::endl;
		out_file << "~MarbleCount" << std::endl;

		out_file << "File end";
		return OutFileName;
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
			//std::cout << "Board loaded from " << InFileName << std::endl;
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

		std::string board_str = extract_substring("!Board", "~Board");
		std::string moves_str = extract_substring("!Moves", "~Moves");
		std::string index_str = extract_substring("!Index", "~Index");
		std::string marble_str = extract_substring("!MarbleCount", "~MarbleCount");

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
					storage_string = "";
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
					storage_string = "";
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
					storage_string = "";
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
					storage_string = "";
					continue;
				}
				storage_string += current_character;
			}
		}

		// Parse index and marble count
		Index = std::stoi(index_str);
		MarbleCount = std::stoi(marble_str);
		HashBoard();
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

	bool IsDepthFirstSearch = false;

	void AddWinningBoard(const std::string& InBoard)
	{

	}

	void AddBoardToQueue(const std::string& InBoard)
	{
		std::string out_file_name = "BoardQueue.txt";
		std::ifstream in_file(out_file_name);
		std::string current_file_content;

		// Read current file content into memory
		if (in_file.is_open())
		{
			std::stringstream buffer;
			buffer << in_file.rdbuf();
			current_file_content = buffer.str();
			in_file.close();
		}

		// Locate existing BoardQueue and LastIndex sections
		size_t board_queue_start = current_file_content.find("!BoardQueue");
		size_t last_index_start = current_file_content.find("!LastIndex");

		// Prepare new content for the file
		std::ofstream out_file(out_file_name);
		if (!out_file.is_open())
			return;

		out_file << "!NextIndex" << std::endl;
		out_file << std::to_string(NextIndex) << std::endl;
		out_file << "~NextIndex" << std::endl;

		// If the BoardQueue section exists, append the new board before LastIndex
		if (board_queue_start != std::string::npos && last_index_start != std::string::npos)
		{
			// Write existing content before the LastIndex
			for (size_t i = 0; i < last_index_start; ++i)
				out_file << current_file_content[i];

			// Append the new board
			out_file << "{";
			out_file << InBoard;
			out_file << "}" << std::endl;
		}
		else
		{
			// Create a new BoardQueue section if it doesn't exist
			out_file << "!BoardQueue" << std::endl;
			out_file << "!FirstIndex" << std::endl;
			out_file << "{";
			out_file << InBoard;
			out_file << "}" << std::endl;
			out_file << "~FirstIndex" << std::endl;
		}

		// Update the LastIndex
		out_file << "!LastIndex" << std::endl;
		out_file << "{";
		out_file << InBoard;
		out_file << "}" << std::endl;
		out_file << "~LastIndex" << std::endl;
		out_file << "~BoardQueue" << std::endl;
	}


	bool QueueHasMembers()const
	{
		std::string out_file_name = "BoardQueue.txt";
		std::ifstream in_file(out_file_name);
		std::string current_file_content;

		// Check if file can be opened
		if (!in_file.is_open())
			return false;

		std::stringstream buffer;
		buffer << in_file.rdbuf();
		current_file_content = buffer.str();
		in_file.close();

		// Check if there is content in the BoardQueue section
		return current_file_content.find(!IsDepthFirstSearch ? "!FirstIndex" : "!LastIndex") != std::string::npos;
	}

	Board* GetBoardFromQueue()
	{
		std::string OutFileName;
		OutFileName += "BoardQueue";
		OutFileName += ".txt";
		std::ifstream saved_board_file(OutFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Board Queue loaded from " << OutFileName << std::endl;
		}
		else
		{
			Board* board = new Board();
			board->SaveBoard();
			return board;
			std::cerr << "Unable to open file for loading: " << OutFileName << std::endl;
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end)
			{
				return InBoardSave.substr(InBoardSave.find(start) + start.length(),
					InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string board_path = !IsDepthFirstSearch ? extract_substring("!FirstIndex", "~FirstIndex") : extract_substring("!LastIndex", "~LastIndex");
		std::string next_index_str = extract_substring("!NextIndex", "~NextIndex");

		NextIndex = std::stoi(next_index_str);

		Board* board = new Board(board_path);
		return board;
	}

	void AddExploredBoard()
	{

	}

	bool BoardExplored(const std::string& InBoard)
	{

	}

	void Save()
	{
		std::cout << "Saving Started" << std::endl;
		// Save data when object is destroyed
		SaveWinningBoards();
		SaveBoardQueue();
		SaveExploredBoards();
		SaveMarbleIterations();
		std::cout << "Saving Complete" << std::endl;
	}

	void Simulate()
	{
		LoadWinningBoards();
		LoadBoardQueue();
		LoadExploredBoards();
		LoadMarbleIterations();
		int iterations = 0;
		while (BoardQueue.empty() == false)
		{
			Board* popped_board;
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
			iterations++;
			if (iterations % 1000 == 0)
			{
				iterations = 0;
				Save();
			}
		}
	}

	void ExploreMoves(Board* InCurrentBoard)
	{
		MarbleIterations[InCurrentBoard->MarbleCount]++;
		if (MarbleIterations[InCurrentBoard->MarbleCount] == 1)
		{
			// New itterations started lets save!
			Save();
		}
		std::cout << "Exploring moves for Board Index " << InCurrentBoard->Index << " Marbles left: " << InCurrentBoard->MarbleCount
			<< " Iterations for marble amount "
			<< MarbleIterations[InCurrentBoard->MarbleCount]
			<< std::endl;

		//CurrentBoard.PrintBoard();

		// Base case: check if it's a winning state
		if (InCurrentBoard->IsWinningState())
		{
			if (DFS)
				ExploredBoards.push_back(InCurrentBoard->Hash);
			BoardQueue.push_back(InCurrentBoard->SaveBoard());
			{
				// Winning Boards are now safe!
				Save();
			}
			return;
		}
		if (InCurrentBoard->HasNoValidMoves())
		{
			if (DFS)
				ExploredBoards.push_back(InCurrentBoard->Hash);
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
				if (DFS && ExploredBoards.Contains(copied_board->Hash))
					continue;

				copied_board->Moves.push_back(next_move);
				copied_board->UpdateIndex();
				//ExploreMoves(copied_board);
				BoardQueue.push_back(copied_board->SaveBoard());
			}
		}
		if (DFS)
			ExploredBoards.push_back(InCurrentBoard->SaveBoard());
	}

	void PrintWinningBoards()
	{
		for (auto board : WinningBoards)
		{
			std::cout << "Printing Moves for board: " << board << std::endl;
			Board display_board;
			const Board loaded_winning_board = Board(board);
			for (const Move& move : loaded_winning_board.Moves)
			{
				display_board.Jump(move);
				std::cout << "Move: ";
				move.PrintMove();
				display_board.PrintBoard();
			}
		}
	}

	void LoadWinningBoards()
	{
		std::string OutFileName;
		OutFileName += "WinningBoards";
		OutFileName += ".txt";
		std::ifstream saved_board_file(OutFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Winning Boards loaded " << OutFileName << std::endl;
		}
		else
		{
			std::cerr << "Unable to open file for loading: " << OutFileName << std::endl;
			BoardQueue.push_back(Board().SaveBoard());
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end)
			{
				return InBoardSave.substr(InBoardSave.find(start) + start.length(),
					InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string winning_boards_str = extract_substring("!WinningBoards", "~WinningBoards");
		std::string storage_string;

		for (char current_character : winning_boards_str)
		{
			if (current_character == '\n')
				continue;
			if (current_character == '{') continue;
			if (current_character == '}')
			{
				WinningBoards.push_back(storage_string);
				storage_string = "";
				continue;
			}
			if (current_character == ';')
				continue;
			storage_string += current_character;
		}
	}

	void LoadBoardQueue()
	{
		std::string OutFileName;
		OutFileName += "BoardQueue";
		OutFileName += ".txt";
		std::ifstream saved_board_file(OutFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Board Queue loaded from " << OutFileName << std::endl;
		}
		else
		{
			BoardQueue.push_back(Board().SaveBoard());
			std::cerr << "Unable to open file for loading: " << OutFileName << std::endl;
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end)
			{
				return InBoardSave.substr(InBoardSave.find(start) + start.length(),
					InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string winning_boards_str = extract_substring("!BoardQueue", "~BoardQueue");
		std::string next_index_str = extract_substring("!NextIndex", "~NextIndex");
		std::string storage_string;

		for (char current_character : winning_boards_str)
		{
			if (current_character == '\n')
				continue;
			if (current_character == '{') continue;
			if (current_character == '}')
			{
				BoardQueue.push_back(storage_string);
				storage_string = "";
				continue;
			}
			if (current_character == ';')
				continue;
			storage_string += current_character;
		}

		NextIndex = std::stoi(next_index_str);
	}

	void LoadExploredBoards()
	{
		std::string OutFileName;
		OutFileName += "ExploredBoards";
		OutFileName += ".txt";
		std::ifstream saved_board_file(OutFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Explored Boards loaded from " << OutFileName << std::endl;
		}
		else
		{
			std::cerr << "Unable to open file for loading: " << OutFileName << std::endl;
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end)
			{
				return InBoardSave.substr(InBoardSave.find(start) + start.length(),
					InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string winning_boards_str = extract_substring("!ExploredBoards", "~ExploredBoards");
		std::string storage_string;

		for (char current_character : winning_boards_str)
		{
			if (current_character == '\n')
				continue;
			if (current_character == '{') continue;
			if (current_character == '}')
			{
				WinningBoards.push_back(storage_string);
				storage_string = "";
				continue;
			}
			if (current_character == ';')
				continue;
			storage_string += current_character;
		}
	}

	void LoadMarbleIterations()
	{
		std::string OutFileName;
		OutFileName += "MarbleIterations";
		OutFileName += ".txt";
		std::ifstream saved_board_file(OutFileName);
		std::string InBoardSave;
		if (saved_board_file.is_open())
		{
			std::stringstream buffer;
			buffer << saved_board_file.rdbuf();  // Read the entire file into the stringstream
			InBoardSave = buffer.str();  // Save the content as a string

			saved_board_file.close();
			std::cout << "Marble Iterations loaded from " << OutFileName << std::endl;
		}
		else
		{
			std::cerr << "Unable to open file for loading: " << OutFileName << std::endl;
			return;
		}

		// Extract substring ranges
		auto extract_substring = [&](const std::string& start, const std::string& end)
			{
				return InBoardSave.substr(InBoardSave.find(start) + start.length(),
					InBoardSave.find(end) - (InBoardSave.find(start) + start.length()));
			};

		std::string winning_boards_str = extract_substring("!MarbleIterations", "~MarbleIterations");

		std::pair<int, int> iteration;
		std::string storage_string;
		int fill_target = 0;

		for (char current_character : winning_boards_str)
		{
			if (current_character == '\n')
				continue;
			if (current_character == '{') continue;
			if (current_character == '}')
			{
				MarbleIterations[iteration.first] = iteration.second;
				storage_string = "";
				iteration = {};
				fill_target = 0;
				continue;
			}
			if (current_character == ';')
			{
				if (fill_target == 0)
					iteration.first = std::stoi(storage_string);
				else if (fill_target == 1)
					iteration.second = std::stoi(storage_string);
				fill_target++;
				storage_string = "";
				continue;
			}
			storage_string += current_character;
		}
	}

	void SaveWinningBoards()
	{
		std::string OutFileName;
		OutFileName += "WinningBoards";
		OutFileName += ".txt";

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

		for (const std::string& winning_board : WinningBoards)
		{
			out_file << winning_board << std::endl;

			Board display_board;
			Board loaded_winning_board = Board(winning_board);
			for (const Move& move : loaded_winning_board.Moves)
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
						if (loaded_winning_board.BoardArray[loc] == SlotStatus::Blocked)
							out_file << "  ";  // Blocked spot
						else if (loaded_winning_board.BoardArray[loc] == SlotStatus::Empty)
							out_file << "X ";  // Empty spot
						else if (loaded_winning_board.BoardArray[loc] == SlotStatus::Marble)
							out_file << "O ";  // Marble present
					}
					out_file << std::endl;  // Newline after each row
				}


				for (const auto& move : loaded_winning_board.Moves)
				{
					out_file << "Starting Board Index: " << move.BoardIndex
						<< " Location: " << move.StartingLocation.ToString()
						<< " Direction: " << DirectionToString(move.MoveDirection) << std::endl;
				}
			}
		}

		out_file << "!Winning Boards" << std::endl;
		for (const std::string& winning_board : WinningBoards)
		{
			out_file << "{" << std::endl;
			out_file << winning_board << std::endl;
			out_file << "}" << std::endl;
		}
		out_file << "~Winning Boards" << std::endl;
	}

	void SaveMarbleIterations()
	{
		std::string OutFileName;
		OutFileName += "MarbleIterations";
		OutFileName += ".txt";


		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

		out_file << "!MarbleIterations" << std::endl;
		for (auto marble_iterations : MarbleIterations)
		{
			out_file << "{" << std::endl;
			out_file << std::to_string(marble_iterations.first) << ";" << std::endl;
			out_file << std::to_string(marble_iterations.second) << ";" << std::endl;
			out_file << "}" << std::endl;
		}
		out_file << "~MarbleIterations" << std::endl;
	}

	void SaveBoardQueue()
	{
		std::string OutFileName;
		OutFileName += "BoardQueue";
		OutFileName += ".txt";

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

		out_file << "!NextIndex" << std::endl;
		out_file << std::to_string(NextIndex) << std::endl;
		out_file << "~NextIndex" << std::endl;

		out_file << "!BoardQueue" << std::endl;
		for (const std::string& member : BoardQueue)
		{
			out_file << "{" << std::endl;
			out_file << member << std::endl;
			out_file << "}" << std::endl;
		}
		out_file << "~BoardQueue" << std::endl;
	}

	void SaveExploredBoards()
	{
		std::string OutFileName;
		OutFileName += "ExploredBoards";
		OutFileName += ".txt";

		std::ofstream out_file(OutFileName);
		if (!out_file.is_open())
			return;

		out_file << "!ExploredBoards" << std::endl;
		for (const std::string& member : ExploredBoards)
		{
			out_file << "{" << std::endl;
			out_file << member << std::endl;
			out_file << "}" << std::endl;
		}
		out_file << "~ExploredBoards" << std::endl;
	}
};

int main()
{
	SolitareSimulation* solitare = new SolitareSimulation();

	solitare->Simulate(false);
	//solitare.SimulatedDFS();

	solitare->PrintWinningBoards();

	return 0;  // Return 0 to indicate successful execution
}