#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "BaseLight.h"
#include "Object.h"

namespace RenderLibrary
{
	namespace Light
	{
		class LightManager
		{
		public:
			LightManager(void);
			~LightManager(void);

			int Initialise();
			int AddLight(RenderLibrary::Light::BaseLight* light);
			int RemoveLight(int number);
			void ProcessLights(RenderLibrary::Object* object);
			Gdiplus::Color MixColours(Gdiplus::Color originalColor, Gdiplus::Color newColor);

			unsigned int index, index2;

			std::vector<RenderLibrary::Light::BaseLight*> _lights;
		};
	}
}

#endif // LIGHTMANAGER_H
