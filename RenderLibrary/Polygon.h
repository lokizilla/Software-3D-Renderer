// Polygon class.

#ifndef POLYGON_H
#define POLYGON_H

#include "Vector.h"
#include <Gdiplus.h>
#include "LightColour.h"

namespace RenderLibrary
{
	class Polygon
	{
	public:

		Polygon(); // Default constructor
		Polygon(int value1, int value2, int value3); // Construct with supplied values
		~Polygon(); // Destructor
		//const Polygon(const Polygon& poly); // Copy constructor

		bool _backface; // Tag for back facing
		MathLibrary::Vector _currentNormal; // Store poly's current normal
		RenderLibrary::Light::LightColour _lightColour;
		Gdiplus::Color _light; // Store poly's light value

		//friend operator =

		int _data[3]; // Hold the individual values
		float _depth; // Hold depth sorting value
	};
}

#endif // POLYGON_H