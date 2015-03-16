#include "stdafx.h"
#include "LightColour.h"

namespace RenderLibrary
{
	namespace Light
	{
		LightColour::LightColour()
		{
			_red = 0;
			_green = 0;
			_blue = 0;
			_alpha = 0;
		}

		LightColour::~LightColour()
		{
		}

		LightColour::LightColour(int red, int green, int blue, int alpha)
		{
			_red = red;
			_green = green;
			_blue = blue;
			_alpha = alpha;
		}
	}
}