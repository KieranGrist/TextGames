#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  // For std::find_if
#include <functional> // For std::function
#include <cstdlib>
using namespace std;

enum MarbleSlot
{
	SlotError,   // Renamed to avoid conflict
	Marble,
	Empty,
	Blocked,
	Label
};

enum Direction
{
	DirectionError,   // Renamed to avoid conflict
	Up,
	Down,
	Left,
	Right
};

struct Location
{
	Location() {}
	Location(char InColumn, int InRow);
	void PrintLocation() const;

	bool operator==(const Location& other) const
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

	void PrintTile() const;
	void PrintDirection(const Direction& InDirection) const;
	static Direction GetDirection(const Location& InStart, const Location& InEnd);
	bool PossibleTileMoves() const;

	MarbleSlot SlotState;
	Location GridLocation;
	vector<Tile*> CaptureLocations;
};

class Board
{
private:
	vector<Tile*> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	void PrintValidSelections() const;
	void Jump(const Location& InJumpStart, const Location& InJumpTo);

	bool SelectTile(const Location& InLocation) const;

	Tile* FindTileByDirection(Location InLocation, Direction InDirection);

	Tile* FindTileByGridLocation(const Location& InLocation) const;



	// Function to find a tile by a predicate
	Tile* FindByPredicate(function<bool(Tile*)> predicate) const;

	void GenerateBoard();

	void GenerateCaptureLocations();

	void PrintBoard() const;
};
