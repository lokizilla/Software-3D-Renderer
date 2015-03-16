#ifndef LIGHTDIRECTIONAL_H
#define LIGHTDIRECTIONAL_H

#include "Vector.h"
#include "Object.h"
#include "BaseLight.h"

namespace RenderLibrary
{
	namespace Light
	{
		class LightDirectional 
			: public RenderLibrary::Light::BaseLight
		{
		public:
			LightDirectional();
			~LightDirectional();

			int Initialise(LightColour colour);
			void CalculateLight(Object& obj);

			unsigned int _index;
			float _totalR, _totalG, _totalB;
			float _tempR, _tempG, _tempB;
			float _dotProduct;
		};
	}
}

#endif // LIGHTDIRECTIONAL_H
