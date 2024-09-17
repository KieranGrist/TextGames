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
	Location() {}

	Location(char InColumn, int InRow);

	Location(const string& InLocation);

	bool Equals(const Location& other) const;

	char Column = '!';  // Column label
	int Row = -1;       // Row number
};

struct Tile
{
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

class MarbleBoard
{
private:
	vector<Tile*> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	bool Equals(const MarbleBoard* other) const;

	void PrintValidSelections() const;

	void Jump(const Location& InJumpStart, const Direction& InJumpDirection);

	bool SelectTile(const Location& InLocation) const;

	void GenerateBoard();

	void CopyBoard(const MarbleBoard* InBoard);

	void GenerateBoardPossibleJumpDirections();

	void GenerateTilePossibleJumpDirections(Tile* InTile);

	int MarbleCount() const;

	MarbleBoard* SimulateMove(const Location& InStartLocation, const Direction& InDirection);

	Tile* FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const;

	Tile* FindTileByGridLocation(const Location& InLocation) const;

	Tile* FindByPredicate(function<bool(Tile*)> predicate) const;

	const vector<Tile*>& GetTiles() const;
};

class MarbleSolitare
{
public:
	virtual void BeginPlay();

	virtual void Update();

	virtual void SanitiseInput(string& InInput);

	virtual void SeperateLocationAndInput(const string& InInput, Location& OutLocation, Direction& OutDirection);

	virtual bool IsPlaying();

	static Direction GetDirection(const Location& InStart, const Location& InEnd);

	static void PrintBoard(const MarbleBoard& InMarbleBoard);

	static void PrintLocation(const Location& InLocation);

	static void PrintTile(const Tile& InTitle);

	static void PrintDirection(const Direction& InDirection);
	
	static void PrintSlotState(const MarbleSlot& InMarbleSlot);

	void EmptyScreen();

	void SimulateBFS();

	void AddBoardToQueue(MarbleBoard* InMarbleBoard);

	MarbleBoard* GetTopBoard();
private:
	// Create a board object
	MarbleBoard Board;

	queue<MarbleBoard*> BoardQueue;
	vector<MarbleBoard*> VisitedBoards;
};