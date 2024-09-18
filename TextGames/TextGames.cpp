#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  // For std::find_if
#include <functional> // For std::function
#include <cstdlib>
#include <queue>
using namespace std;

enum class MarbleSlot
{
	SlotError,   // Renamed to avoid conflict
	Marble,
	Empty,
	Blocked,
	Label
};

enum class Direction
{
	DirectionError,   // Renamed to avoid conflict
	Up,
	Down,
	Left,
	Right
};

struct Location
{
	Location()
	{
	}

	Location(char InColumn, int InRow)
	{
		Column = InColumn;
		Row = InRow;
	}

	Location(const string& InLocation)
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

	bool Equals(const Location& other) const
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
		SlotState = MarbleSlot::SlotError;
		GridLocation = Location();
	}

	bool Equals(const Tile* other) const
	{
		//cout << "Comparing tiles at location: ";
		//MarbleSolitare::PrintLocation(GridLocation);
	//cout << std::endl;

		// Check if SlotState is equal
		if (SlotState != other->GetSlotState())
		{
			//cout << "SlotState mismatch: ";
			//MarbleSolitare::PrintSlotState(SlotState);
			//cout << " vs ";
			//MarbleSolitare::PrintSlotState(other->GetSlotState());
			//cout << std::endl;
			return false;
		}

		// Check if GridLocation is equal
		if (!GridLocation.Equals(other->GetGridLocation()))
		{
			//cout << "GridLocation mismatch: ";
			//MarbleSolitare::PrintLocation(GridLocation);
			//cout << " vs ";
			///MarbleSolitare::PrintLocation(other->GetGridLocation());
			//cout << std::endl;
			return false;
		}
		// Compare each tile

		if (PossibleJumpDirections.size() != other->PossibleJumpDirections.size())
		{
			//cout << "Size mismatch: " << std::endl;
			return false;
		}
		for (size_t i = 0; i < PossibleJumpDirections.size(); ++i)
		{
			//cout << "Comparing PossibleJumpDirection at index " << i << std::endl;
			if (PossibleJumpDirections[i] != other->PossibleJumpDirections[i])  // Corrected this from your code: should be if (!Equals(...))
			{
				//cout << "Direction mismatch: ";
				//MarbleSolitare::PrintDirection(PossibleJumpDirections[i]);
				//cout << " vs ";
				//MarbleSolitare::PrintDirection(other->PossibleJumpDirections[i]);
				//cout << std::endl;
				return false;
			}
		}
		//cout << "Tiles are equal!" << std::endl;
		return true;
	}

	void PrintPossibleMoves() const
	{
		if (SlotState != MarbleSlot::Marble)
			return;
		cout << " Possible Jumps" << endl;
		for (auto direction : PossibleJumpDirections)
		{
			MarbleSolitare::PrintLocation(GridLocation);
			cout << " Can Jump/Capture ";
			MarbleSolitare::PrintDirection(direction);
			cout << endl;
		}
	}

	const MarbleSlot& GetSlotState() const
	{
		return SlotState;
	}

	const Location& GetGridLocation() const
	{
		return GridLocation;
	}

	const vector<Direction>& GetPossibleJumpDirections() const
	{
		return PossibleJumpDirections;
	}

	bool HasPossiblePossibleJumpDirection() const
	{
		return PossibleJumpDirections.size() > 0;
	}

	void SetSlotState(const MarbleSlot& InMarbleSlot)
	{
		SlotState = InMarbleSlot;
	}

	void SetGridLocation(const Location& InGridLocation)
	{
		GridLocation = InGridLocation;
	}

	void ClearPossibleJumpDirections()
	{
		PossibleJumpDirections.clear();
	}

	void AddPossibleJumpDirection(const Direction& InDirection)
	{
		PossibleJumpDirections.push_back(InDirection);
	}

private:
	MarbleSlot SlotState;
	Location GridLocation;
	vector<Direction> PossibleJumpDirections;
};

class MarbleBoard
{
private:
	vector<Tile*> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	bool Equals(const MarbleBoard* other) const
	{
		cout << "Comparing two boards..." << std::endl;
		auto board = GetValidTiles();
		auto other_board_tiles = other->GetValidTiles();

		// Check if sizes are the same
		if (board.size() != other_board_tiles.size())
		{
			cout << "Tile size mismatch: " << board.size() << " vs " << other->Tiles.size() << std::endl;
			return false;
		}

		// Compare each tile
		for (size_t i = 0; i < other_board_tiles.size(); ++i)
		{
			cout << "Comparing Tile at index " << i << std::endl;
			if (!board[i]->Equals(other_board_tiles[i]))  // Corrected this from your code: should be if (!Equals(...))
			{
				cout << "Tiles at index " << i << " are not equal!" << std::endl;
				return false;
			}
		}

		// cout << "Boards are equal!" << std::endl;
		return true;
	}

	void PrintValidSelections() const
	{
		for (auto tile : Tiles)
		{
			tile->PrintPossibleMoves();
		}
	}

	void Jump(const Location& InJumpStart, const Direction& InJumpDirection)
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

	bool SelectTile(const Location& InLocation) const
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

	void GenerateBoard()
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

	void CopyBoard(const MarbleBoard* InBoard)
	{
		GenerateBoard();
		for (int i = 0; i < Tiles.size(); ++i)
		{
			Tiles[i]->SetGridLocation(InBoard->Tiles[i]->GetGridLocation());
			Tiles[i]->SetSlotState(InBoard->Tiles[i]->GetSlotState());
		}
	}

	void GenerateBoardPossibleJumpDirections()
	{
		for (auto tile : Tiles)
		{
			GenerateTilePossibleJumpDirections(tile);
		}
	}

	void GenerateTilePossibleJumpDirections(Tile* InTile)
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

	int MarbleCount() const
	{
		int count = 0;
		for (auto tile : Tiles)
		{
			if (tile->GetSlotState() == MarbleSlot::Marble)
				count++;
		}
		return count;
	}

	Tile* FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const
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

	Tile* FindTileByGridLocation(const Location& InLocation) const
	{
		return FindByPredicate([InLocation](Tile* t)
			{
				return t->GetGridLocation().Equals(InLocation);
			});
	}

	Tile* FindByPredicate(function<bool(Tile*)> predicate) const
	{
		auto it = find_if(Tiles.begin(), Tiles.end(), predicate);
		if (it != Tiles.end())
			return *it;
		return nullptr;  // Return nullptr if no tile is found
	}

	const vector<Tile*>& GetTiles() const
	{
		return Tiles;
	}

	const vector<Tile*>& GetValidTiles() const
	{
		static vector<Tile*> marbles;
		marbles.clear();  // Clear the vector before adding new elements

		// Define the predicate to filter marbles
		auto isMarble = [](const Tile* tile) {
			return tile->GetSlotState() != MarbleSlot::Blocked &&
				tile->GetSlotState() != MarbleSlot::SlotError &&
				tile->GetSlotState() != MarbleSlot::Label;
		};

		// Filter the tiles based on the predicate
		for (auto tile : Tiles)
		{
			if (isMarble(tile)) {
				marbles.push_back(tile);
			}
		}

		return marbles;
	}

	const vector<Tile*>& GetMoveableMarbles() const
	{
		static vector<Tile*> marbles;
		marbles.clear();  // Clear the vector before adding new elements

		// Define the predicate to filter marbles
		auto is_moveable = [](const Tile* tile) {
			return tile->HasPossiblePossibleJumpDirection();
		};

		// Filter the tiles based on the predicate
		for (auto tile : Tiles)
		{
			if (is_moveable(tile)) {
				marbles.push_back(tile);
			}
		}

		return marbles;
	}
};

class MarbleSolitare
{
public:
	virtual void BeginPlay()
	{
		EmptyScreen();
		Board.GenerateBoard();
	}

	virtual void Update()
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

	virtual void SanitiseInput(string& InInput)
	{
		// Use std::transform to convert each character to uppercase
		transform(InInput.begin(), InInput.end(), InInput.begin(), ::toupper);

		// Remove spaces
		InInput.erase(remove_if(InInput.begin(), InInput.end(), ::isspace), InInput.end());
	}

	virtual void SeperateLocationAndInput(const string& InInput, Location& OutLocation, Direction& OutDirection)
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

	virtual bool IsPlaying()
	{
		return Board.MarbleCount() > 1;
	}

	static MarbleBoard* SimulateMove(const MarbleBoard* InMarbleBoard, const Location& InStartLocation, const Direction& InDirection)
	{
		//MarbleSolitare::PrintLocation(InStartLocation);
		//cout << " Jumps in direction ";
		//MarbleSolitare::PrintDirection(InDirection);
		//cout << endl;

		MarbleBoard* new_board = new MarbleBoard();
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

	static Direction GetDirection(const Location& InStart, const Location& InEnd)
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

	static void PrintBoard(const MarbleBoard& InMarbleBoard)
	{
		// Print the board with row numbers
		for (auto tile : InMarbleBoard.GetTiles())
		{
			PrintTile(*tile);
		}
		cout << endl;
	}

	static void PrintLocation(const Location& InLocation)
	{
		cout << "Location:" << InLocation.Column << InLocation.Row;
	}

	static void PrintTile(const Tile& InTitle)
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

	static void PrintDirection(const Direction& InDirection)
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

	static void PrintSlotState(const MarbleSlot& InMarbleSlot)
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

	void EmptyScreen()
	{
#ifdef _WIN32
		system("cls");  // Windows
#else
		system("clear");  // Linux/Unix/OSX
#endif
	}

	void SimulateBFS()
	{
		MarbleBoard* start_board = new MarbleBoard();
		start_board->GenerateBoard();
		start_board->GenerateBoardPossibleJumpDirections();

		//queue<MarbleBoard*> board_queue;  // Queue for BFS
		AddBoardToQueue(start_board);
		int wins = 0;
		int boards = 0;
		int jumps = 0;
		while (!BoardQueue.empty())
		{
			MarbleBoard* current_board = GetTopBoard();
			boards++;
			//MarbleSolitare::PrintBoard(*current_board);
			// Collect statistics for the current board state
			int marbleCount = current_board->MarbleCount();
			cout << "Number of marbles remaining: " << marbleCount << endl;
			cout << "Boards: " << boards << " Board left: " << BoardQueue.size() << endl;
			cout << "Wins: " << wins << " Boards: " << boards << " Jumps " << jumps << endl;

			//	current_board->PrintValidSelections();

			// Generate possible moves from the current board state

			auto moveable_marbles = current_board->GetMoveableMarbles();
			for (auto tile : moveable_marbles)
			{
				for (const auto& direction : tile->GetPossibleJumpDirections())
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

	void AddBoardToQueue(MarbleBoard* InMarbleBoard)
	{
		//cout << "Attempting to add board to queue..." << std::endl;

			// Check if the board is already in the visited boards
		for (MarbleBoard* visited_board : VisitedBoards)
		{
			if (visited_board->Equals(InMarbleBoard))
			{
				//cout << "Board is already in visited list, skipping." << std::endl;
				return;
			}
		}

		for (MarbleBoard* visited_board : BoardQueue)
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

	MarbleBoard* GetTopBoard()
	{
		MarbleBoard* popped_board = BoardQueue.front();
		BoardQueue.erase(BoardQueue.begin());
		VisitedBoards.push_back(popped_board);
		popped_board->GenerateBoardPossibleJumpDirections();
		return popped_board;
	}
private:
	// Create a board object
	MarbleBoard Board;

	vector<MarbleBoard*> BoardQueue;
	vector<MarbleBoard*> VisitedBoards;
};

int main()
{
	MarbleSolitare Solitare;
	Solitare.SimulateBFS();
	/*Solitare.BeginPlay();
	while (Solitare.IsPlaying())
	{
		Solitare.Update();
	}*/
	return 0;
}
