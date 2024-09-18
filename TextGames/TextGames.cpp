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



int main()
{
	MarbleSolitare Solitare;
	Solitare.SimulateBFS();
	/*Solitare.BeginPlay();
	while (Solitare.IsPlaying())
	{
		Solitare.Update();
	}*/
	return 0;
}
