#ifndef RANDOMINT_H
#define RANDOMINT_H

namespace MathLibrary
{
	class RandomInt
	{
	public:
		RandomInt();
		~RandomInt();

		int GetRandomNumber();
		int GetRangedRandomNumber(int start, int end);
	};
}

#endif // RANDOMINT_H
