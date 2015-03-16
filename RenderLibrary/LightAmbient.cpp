#include "StdAfx.h"
#include "LightAmbient.h"

namespace RenderLibrary
{
	namespace Light
	{
		LightAmbient::LightAmbient(void)
		{
		}

		LightAmbient::~LightAmbient(void)
		{
		}

		int LightAmbient::Initialise(RenderLibrary::Light::LightColour colour)
		{
			_colour = new RenderLibrary::Light::LightColour(colour);

			return 1; // debug
		}

		void LightAmbient::CalculateLight(Object& obj)
		{
			int index = 0;

			for(index = 0; index < obj.GetNumberOfPolys(); index++) // For all polys in object
			{
				obj._polys[index]->_lightColour._alpha = _colour->_alpha;
				obj._polys[index]->_lightColour._red = _colour->_red;
				obj._polys[index]->_lightColour._green = _colour->_green;
				obj._polys[index]->_lightColour._blue = _colour->_blue;
			}
		}
	}
}