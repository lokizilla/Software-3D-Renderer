#ifndef LIGHTAMBIENT_H
#define LIGHTAMBIENT_H

#include "Object.h"
#include "BaseLight.h"

namespace RenderLibrary
{
	namespace Light
	{
		class LightAmbient
			: public RenderLibrary::Light::BaseLight
		{
		public:
			LightAmbient(void);
			~LightAmbient(void);

			int Initialise(LightColour colour);
			void CalculateLight(Object& obj);
		};
	}
}

#endif // LIGHTAMBIENT_H
