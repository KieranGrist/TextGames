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

	static MarbleBoard* SimulateMove(const MarbleBoard* InMarbleBoard, const Location& InStartLocation, const Direction& InDirection);

	static Direction GetDirection(const Location& InStart, const Location& InEnd);

	static void PrintDirection(const Direction& InDirection);

	static void PrintSlotState(const MarbleSlot& InMarbleSlot);

	static void PrintLocation(const Location& InLocation);

	static void PrintTile(const Tile& InTitle);

	static void PrintBoard(const MarbleBoard& InMarbleBoard);

	void EmptyScreen();

	void SimulateBFS();

	void AddBoardToQueue(MarbleBoard* InMarbleBoard);
	
	bool BoardPressent(MarbleBoard* InMarbleBoard);

	bool FindBoardByPredicte(function<bool(MarbleBoard*)> InPredicate) const;

	MarbleBoard* GetTopBoard();
private:
	// Create a board object
	MarbleBoard Board;

	vector<MarbleBoard*> BoardQueue;

	vector<MarbleBoard*> VisitedBoards;
};