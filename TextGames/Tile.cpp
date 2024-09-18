#include "Tile.h"
Tile::Tile()
{
	SlotState = MarbleSlot::SlotError;
	GridLocation = Location();
}

bool Tile::Equals(const Tile* other) const
{
	//cout << "Comparing tiles at location: ";
	//PrintLocation(GridLocation);
//cout << std::endl;

	// Check if SlotState is equal
	if (SlotState != other->GetSlotState())
	{
		//cout << "SlotState mismatch: ";
		//PrintSlotState(SlotState);
		//cout << " vs ";
		//PrintSlotState(other->GetSlotState());
		//cout << std::endl;
		return false;
	}

	// Check if GridLocation is equal
	if (!GridLocation.Equals(other->GetGridLocation()))
	{
		//cout << "GridLocation mismatch: ";
		//PrintLocation(GridLocation);
		//cout << " vs ";
		///PrintLocation(other->GetGridLocation());
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
			//PrintDirection(PossibleJumpDirections[i]);
			//cout << " vs ";
			//PrintDirection(other->PossibleJumpDirections[i]);
			//cout << std::endl;
			return false;
		}
	}
	//cout << "Tiles are equal!" << std::endl;
	return true;
}

void Tile::PrintPossibleMoves() const
{
	if (SlotState != MarbleSlot::Marble)
		return;
	cout << " Possible Jumps" << endl;
	for (auto direction : PossibleJumpDirections)
	{
		PrintLocation(GridLocation);
		cout << " Can Jump/Capture ";
		PrintDirection(direction);
		cout << endl;
	}
}

const MarbleSlot& Tile::GetSlotState() const
{
	return SlotState;
}

const Location& Tile::GetGridLocation() const
{
	return GridLocation;
}

const vector<Direction>& Tile::GetPossibleJumpDirections() const
{
	return PossibleJumpDirections;
}

bool Tile::HasPossiblePossibleJumpDirection() const
{
	return PossibleJumpDirections.size() > 0;
}

void Tile::SetSlotState(const MarbleSlot& InMarbleSlot)
{
	SlotState = InMarbleSlot;
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
