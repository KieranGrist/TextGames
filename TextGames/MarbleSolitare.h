#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  // For std::find_if
#include <functional> // For std::function
#include <cstdlib>
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
	Location() {}

	Location(char InColumn, int InRow);

	Location(const string& InLocation);

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
	Tile();

	void PrintTile() const;

	void PrintDirection(const Direction& InDirection) const;

	bool PrintPossibleMoves() const;

	static Direction GetDirection(const Location& InStart, const Location& InEnd);

	const MarbleSlot& GetSlotState() const;

	const Location& GetGridLocation() const;

	const vector<Tile*>& GetCapturedLocations() const;

	bool HasPossibleCaptureLocations() const;

	void SetSlotState(const MarbleSlot& InMarbleSlot);

	void SetGridLocation(const Location& InGridLocation);

	void ClearCapturedLocations();

	void AddCapturedLocations(Tile* InLocation);

private:
	MarbleSlot SlotState;
	Location GridLocation;
	vector<Tile*> CaptureLocations;
};

class MarbleBoard
{
private:
	vector<Tile*> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	void PrintValidSelections() const;
	
	void Jump(const Location& InJumpStart, const Direction& InJumpDirection);

	bool SelectTile(const Location& InLocation) const;

	void GenerateBoard();

	void GenerateCaptureLocations();

	void PrintBoard() const;

	int MarbleCount() const;

	Tile* FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const;

	Tile* FindTileByGridLocation(const Location& InLocation) const;

	Tile* FindByPredicate(function<bool(Tile*)> predicate) const;
};

class MarbleSolitare
{
public:
	virtual void BeginPlay();

	virtual void Update();

	virtual void SanitiseInput(string& InInput);

	virtual void SeperateLocationAndInput(const string& InInput, Location& OutLocation, Direction& OutDirection);

	virtual bool IsPlaying();

	void EmptyScreen();

private:
	// Create a board object
	MarbleBoard Board;
};