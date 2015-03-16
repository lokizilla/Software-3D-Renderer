#include "StdAfx.h"
#include "LightDirectional.h"
#include "LightManager.h"

namespace RenderLibrary
{
	namespace Light
	{
		LightDirectional::LightDirectional()
		{

		}

		LightDirectional::~LightDirectional()
		{
		}

		int LightDirectional::Initialise(LightColour colour)
		{
			_direction = new MathLibrary::Vector();
			_colour = new RenderLibrary::Light::LightColour(colour);

			return 1; // debug
		}

		void LightDirectional::CalculateLight(Object& obj)
		{
			for(_index = 0; (int)_index < obj.GetNumberOfPolys(); ++_index) // for each poly in our object...
			{
				if (obj._polys[_index]->_backface == false) // Process only where not back facing (saves time!)
				{
					// reset total r,g,b values to black
					_totalR = 0;
					_totalG = 0;
					_totalB = 0;

					// get the light intensities components (r,g,b) into temp r,g,b
					_tempR = (float)_colour->_red;
					_tempG = (float)_colour->_green;
					_tempB = (float)_colour->_blue;					

					// modulate (multiply) the light intensities with corresponding reflectance co-efficients on object
					_tempR *= obj._reflectivity._red;
					_tempG *= obj._reflectivity._green;
					_tempB *= obj._reflectivity._blue;

					// attenuate the rgb values with the Lambertian attenuation ("take the dot-product of the Polygon normal and the vector to the light source from the current polygon")
					obj._polys[_index]->_currentNormal.Normalise();
					_dotProduct = obj._polys[_index]->_currentNormal.DotProduct(_direction->Normalise());

					 if (_dotProduct > 1.0f)
						_dotProduct = 1.0f;

					if (_dotProduct < -1.0f)
						_dotProduct = -1.0f;

					_tempR *= _dotProduct;
					_tempG *= _dotProduct;
					_tempB *= _dotProduct;

					// add light contribution in temps r,g,b to totals
					_totalR += _tempR;
					_totalG += _tempG;
					_totalB += _tempB;

					// clamp total r,g,b values each [0-255 for 8-bit colour channels)
					if (_totalR > 255)
						_totalR = 255;
					if (_totalG > 255)
						_totalG = 255;
					if (_totalB > 255)
						_totalB = 255;
					if (_totalR < 0)
						_totalR = 0;
					if (_totalG < 0)
						_totalG = 0;
					if (_totalB < 0)
						_totalB = 0;

					// build ARGB from total r,g,b and store lit colour in Polygon (mixing colours from other directional (diffuse) lights)					
					obj._polys[_index]->_lightColour._alpha += _colour->_alpha;
					obj._polys[_index]->_lightColour._red += (int)_totalR;
					obj._polys[_index]->_lightColour._green += (int)_totalG;
					obj._polys[_index]->_lightColour._blue += (int)_totalB;
				}
			}
		}
	}
}

// ambient: initial rgb * scaling value (ambient)

// Lambert's law: initial rgb * light.normal (dp) * scaling value (diffuse)

// specular: opposite of diffuse(?):
