#pragma once
#include "Location.h"
class Tile;

class MarbleBoard
{
private:
	vector<unique_ptr<Tile>> Tiles;  // Tiles in the board
	int MaxCol = 7;
	int MaxRow = 7;

public:
	MarbleBoard() {}

	bool Equals(const vector<unique_ptr<MarbleBoard>> other) const;

	void PrintValidSelections() const;

	void Jump(const Location& InJumpStart, const Direction& InJumpDirection);

	bool SelectTile(const Location& InLocation) const;

	void GenerateBoard();

	void CopyBoard(const vector<unique_ptr<MarbleBoard>> InBoard);

	void GenerateBoardPossibleJumpDirections();

	void GenerateTilePossibleJumpDirections(Tile* InTile);

	int MarbleCount() const;

	Tile* FindTileByDirection(Location InLocation, const Direction& InDirection, int InDistance) const;

	Tile* FindTileByGridLocation(const Location& InLocation) const;

	Tile* FindByPredicate(function<bool(Tile*)> InPredicate) const;

	const vector<unique_ptr<Tile>>& GetTiles() const;

	vector<unique_ptr<Tile>>FilterTilesByPredicate(function<bool(Tile*)> InPredicate) const;

	vector<unique_ptr<Tile>> GetValidTiles() const;

	vector<unique_ptr<Tile>> GetMarbles() const;
	
	vector<unique_ptr<Tile>> GetMoveableMarbles() const;
};