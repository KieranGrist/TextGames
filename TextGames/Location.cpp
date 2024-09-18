#include "Location.h"

	Location::Location()
	{
	}

	Location::Location(char InColumn, int InRow)
	{
		Column = InColumn;
		Row = InRow;
	}

	Location::Location(const string& InLocation)
	{
		// Check if the input string has the expected length
		if (InLocation.length() >= 2)
		{
			Column = InLocation[0];  // First character is the column (e.g., 'D')
			Row = InLocation[1] - '0';  // Second character is the row (e.g., '4')
		}
		else
		{
			// Handle error: invalid location string
			Column = '!';  // Or any default/error value
			Row = -1;      // Or any default/error value
		}
	}

	bool Location::Equals(const Location& other) const
	{
		return Column == other.Column && Row == other.Row;
	}

