#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "LightColour.h"
#include "Object.h"

namespace RenderLibrary
{
	namespace Light
	{
		class BaseLight
		{
		public:
			BaseLight(void);
			~BaseLight(void);

			virtual int Initialise(RenderLibrary::Light::LightColour colour);
			virtual void CalculateLight(RenderLibrary::Object& obj);

			MathLibrary::Vector* _position;
			MathLibrary::Vector* _direction;

			RenderLibrary::Light::LightColour* _colour;
		};
	}
}

#endif // BASELIGHT_H
