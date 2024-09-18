#include "MarbleBoard.h"

	bool MarbleBoard::Equals(const MarbleBoard* other) const
	{
		//cout << "Comparing two boards..." << std::endl;
		auto board = GetValidTiles();
		auto other_board_tiles = other->GetValidTiles();

		// Check if sizes are the same
		if (board.size() != other_board_tiles.size())
		{
			//cout << "Tile size mismatch: " << board.size() << " vs " << other->Tiles.size() << std::endl;
			return false;
		}

		// Compare each tile
		for (size_t i = 0; i < other_board_tiles.size(); ++i)
		{
			//cout << "Comparing Tile at index " << i << std::endl;
			if (!board[i]->Equals(other_board_tiles[i]))  // Corrected this from your code: should be if (!Equals(...))
			{
				//cout << "Tiles at index " << i << " are not equal!" << std::endl;
				return false;
			}
		}

		// cout << "Boards are equal!" << std::endl;
		return true;
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
			PrintLocation(InLocation);
			cout << " is an invalid selection" << endl;
			return false;
		}
		if (!tile->HasPossiblePossibleJumpDirection())
		{
			PrintLocation(InLocation);
			cout << " has no possible Jumps/Captures" << endl;
			return false;
		}
		PrintLocation(InLocation);
		cout << " selected" << endl;
		return true;
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

	void MarbleBoard::CopyBoard(const MarbleBoard* InBoard)
	{
		GenerateBoard();
		for (int i = 0; i < Tiles.size(); ++i)
		{
			Tiles[i]->SetGridLocation(InBoard->Tiles[i]->GetGridLocation());
			Tiles[i]->SetSlotState(InBoard->Tiles[i]->GetSlotState());
		}
	}

	void MarbleBoard::GenerateBoardPossibleJumpDirections()
	{
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

	Tile* MarbleBoard::indTileByGridLocation(const Location& InLocation) const
	{
		return FindByPredicate([InLocation](Tile* t)
			{
				return t->GetGridLocation().Equals(InLocation);
			});
	}

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

	const vector<Tile*>& MarbleBoard::GetValidTiles() const
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

	const vector<Tile*>& MarbleBoard::GetMoveableMarbles() const
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
