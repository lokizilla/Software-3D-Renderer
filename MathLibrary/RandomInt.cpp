#include "RandomInt.h"

#include <iostream> 
#include <ctime> 
#include <cstdlib>

namespace MathLibrary
{
	RandomInt::RandomInt()
	{
		srand((unsigned)time(0)); 
	}

	RandomInt::~RandomInt()
	{
	}

	int RandomInt::GetRandomNumber()
	{
		return rand();
	}

	int RandomInt::GetRangedRandomNumber(int start, int end)
	{
		int range = ((start * 2) - (end * 2));

		return (int)(start + range*rand()/(RAND_MAX + 1.0) - (start - end)); 
	}
}