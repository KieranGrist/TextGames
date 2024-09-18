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

enum struct MarbleSlot
{
	SlotError,   // Renamed to avoid conflict
	Marble,
	Empty,
	Blocked,
	Label
};

enum struct Direction
{
	DirectionError,   // Renamed to avoid conflict
	Up,
	Down,
	Left,
	Right
};

struct Location
{
public:
	Location();

	Location(char InColumn, int InRow);

	Location(const string& InLocation);

	bool Equals(const Location& other) const;

	char Column = '!';  // Column label
	int Row = -1;       // Row number
};

