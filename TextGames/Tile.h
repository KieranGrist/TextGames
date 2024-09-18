#pragma once
#include "Location.h";

class Tile
{
public:
	Tile();

	bool Equals(const Tile* other) const;

	void PrintPossibleMoves() const;

	const MarbleSlot& GetSlotState() const;

	const Location& GetGridLocation() const;

	const vector<Direction>& GetPossibleJumpDirections() const;

	bool HasPossiblePossibleJumpDirection() const;

	void SetSlotState(const MarbleSlot& InMarbleSlot);

	void SetGridLocation(const Location& InGridLocation);

	void ClearPossibleJumpDirections();

	void AddPossibleJumpDirection(const Direction& InDirection);

private:
	MarbleSlot SlotState;
	Location GridLocation;
	vector<Direction> PossibleJumpDirections;
};