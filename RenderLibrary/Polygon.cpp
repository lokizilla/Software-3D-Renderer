#include "stdafx.h"
#include "Polygon.h"

namespace RenderLibrary
{
	Polygon::Polygon()
	{
		_data[0] = 0;
		_data[1] = 0;
		_data[2] = 0;

		_backface = false;
		_depth = 0;
	}

	Polygon::Polygon(int value1, int value2, int value3)
	{
		_data[0] = value1;
		_data[1] = value2;
		_data[2] = value3;

		_backface = false;
		_depth = 0;
	}

	Polygon::~Polygon()
	{

	}

	//Polygon::Polygon(const RenderLibrary::Polygon &poly)
	//{

	//}
}