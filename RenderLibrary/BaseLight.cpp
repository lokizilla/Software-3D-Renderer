#include "StdAfx.h"
#include "BaseLight.h"

namespace RenderLibrary
{
	namespace Light
	{
		BaseLight::BaseLight(void)
		{
		}

		BaseLight::~BaseLight(void)
		{
		}

		int BaseLight::Initialise(LightColour colour)
		{
			return 1; // debug
		}

		void BaseLight::CalculateLight(Object& obj)
		{
		}
	}
}
