#include "MarbleSolitare.h"

void EmptyScreen()
{
#ifdef _WIN32
		system("cls");  // Windows
#else
		system("clear");  // Linux/Unix/OSX
#endif
}

int main()
{
	// Create a board object
	Board board;
	EmptyScreen();
	// Generate and print the board
	board.GenerateBoard();
	board.PrintBoard();
	board.PrintValidSelections();

	return 0;
}
