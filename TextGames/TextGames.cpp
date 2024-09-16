#include "MarbleSolitare.h"



int main()
{
	MarbleSolitare Solitare;
	Solitare.BeginPlay();
	while (Solitare.IsPlaying())
	{
		Solitare.Update();
	}
	return 0;
}
