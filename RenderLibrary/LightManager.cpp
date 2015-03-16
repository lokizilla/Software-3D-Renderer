#include "StdAfx.h"
#include "LightManager.h"

namespace RenderLibrary
{
	namespace Light
	{
		LightManager::LightManager(void)
		{
		}

		LightManager::~LightManager(void)
		{
			for (index = 0; index < _lights.size(); index++)
			{
				delete _lights[index];
			}

			_lights.clear();
		}

		int LightManager::Initialise()
		{
			return 0; // debug
		}

		int LightManager::AddLight(RenderLibrary::Light::BaseLight* light)
		{
			_lights.push_back(light);

			return 0; // debug
		}

		int LightManager::RemoveLight(int number)
		{
			delete _lights[number];
			_lights.erase(_lights.begin() + number);

			return 0; // debug
		}

		void LightManager::ProcessLights(RenderLibrary::Object* object)
		{
			for (index = 0; index < _lights.size(); index++) 
			{
				_lights[index]->CalculateLight(*object);
			}

			for (index = 0; index < object->_polys.size(); index++)
			{
				// Clamp light values
				if (object->_polys[index]->_lightColour._alpha > 255)
					object->_polys[index]->_lightColour._alpha = 255;
				if (object->_polys[index]->_lightColour._red > 255)
					object->_polys[index]->_lightColour._red = 255;
				if (object->_polys[index]->_lightColour._green > 255)
					object->_polys[index]->_lightColour._green = 255;
				if (object->_polys[index]->_lightColour._blue > 255)
					object->_polys[index]->_lightColour._blue = 255;

				if (object->_polys[index]->_lightColour._alpha < 0)
					object->_polys[index]->_lightColour._alpha = 0;
				if (object->_polys[index]->_lightColour._red < 0)
					object->_polys[index]->_lightColour._red = 0;
				if (object->_polys[index]->_lightColour._green < 0)
					object->_polys[index]->_lightColour._green = 0;
				if (object->_polys[index]->_lightColour._blue < 0)
					object->_polys[index]->_lightColour._blue = 0;
				
				object->_polys[index]->_light = Gdiplus::Color::MakeARGB(
					object->_polys[index]->_lightColour._alpha, 
					object->_polys[index]->_lightColour._red, 
					object->_polys[index]->_lightColour._green, 
					object->_polys[index]->_lightColour._blue);

				if (_lights.size() == 0) // Return the objects to black where there are no lights to process
				{
					object->_polys[index]->_lightColour._alpha = 0;
					object->_polys[index]->_lightColour._red = 0;
					object->_polys[index]->_lightColour._green = 0;
					object->_polys[index]->_lightColour._blue = 0;
				}
			}
		}
	}
}
