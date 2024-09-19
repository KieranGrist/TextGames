#pragma once
#include "Location.h"
#include "MarbleBoard.h"
class Tile;

class MarbleSolitare
{
public:
	virtual void BeginPlay();

	virtual void Update();

	virtual void SanitiseInput(string& InInput);

	virtual void SeperateLocationAndInput(const string& InInput, Location& OutLocation, Direction& OutDirection);

	virtual bool IsPlaying();

	static vector<unique_ptr<MarbleBoard>> SimulateMove(const vector<unique_ptr<MarbleBoard>> InMarbleBoard, const Location& InStartLocation, const Direction& InDirection);

	static Direction GetDirection(const Location& InStart, const Location& InEnd);

	static void PrintDirection(const Direction& InDirection);

	static void PrintSlotState(const MarbleSlot& InMarbleSlot);

	static void PrintLocation(const Location& InLocation);

	static void PrintTile(const Tile& InTitle);

	static void PrintBoard(const MarbleBoard& InMarbleBoard);

	void EmptyScreen();

	void SimulateBFS();

	void AddBoardToQueue(const vector<unique_ptr<MarbleBoard>>& InMarbleBoard);
	
	bool BoardPressent(const vector<unique_ptr<MarbleBoard>&> InMarbleBoard);

	bool FindBoardByPredicte(function<bool(vector<unique_ptr<MarbleBoard>>)> InPredicate) const;

	vector<unique_ptr<MarbleBoard>> GetTopBoard();
private:
	// Create a board object
	MarbleBoard Board;

	vector<vector<unique_ptr<MarbleBoard>>> BoardQueue;

	vector<vector<unique_ptr<MarbleBoard>>> VisitedBoards;
};