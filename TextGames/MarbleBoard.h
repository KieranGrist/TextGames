#pragma once
#include "Location.h"
class Tile;

class MarbleBoard
{
private:
	vector<Tile*> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	MarbleBoard() {}

	bool Equals(const MarbleBoard* other) const;

	void PrintValidSelections() const;

	void Jump(const Location& InJumpStart, const Direction& InJumpDirection);

	bool SelectTile(const Location& InLocation) const;

	void GenerateBoard();

	void CopyBoard(const MarbleBoard* InBoard);

	void GenerateBoardPossibleJumpDirections();

	void GenerateTilePossibleJumpDirections(Tile* InTile);

	int MarbleCount() const;

	Tile* FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const;

	Tile* FindTileByGridLocation(const Location& InLocation) const;

	Tile* FindByPredicate(function<bool(Tile*)> InPredicate) const;

	const vector<Tile*>& GetTiles() const;

	vector<Tile*>FilterTilesByPredicate(function<bool(Tile*)> InPredicate) const;

	vector<Tile*> GetValidTiles() const;

	vector<Tile*> GetMarbles() const;
	
	vector<Tile*> GetMoveableMarbles() const;
};