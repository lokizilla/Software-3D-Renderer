// Rasteriser implementation file.

#include "stdafx.h"
#include "RasteriserGDI.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace RenderLibrary
{
	RasteriserGDI::RasteriserGDI()
	{
		// Initialise() must be called
	}

	RasteriserGDI::~RasteriserGDI()
	{
		delete _pen;
		delete _statusFont;
		delete _pointFont;
		delete _statusFontBrush;
		delete _bitmap; 
		delete _graphics;
		delete [] _scanlines;
	}

	int RasteriserGDI::Initialise(int viewportWidth, int viewportHeight)
	{
		_bitmap = new Gdiplus::Bitmap(viewportWidth, viewportHeight, PixelFormat32bppARGB);
		_graphics = new Gdiplus::Graphics(_bitmap);	
		_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255), 1);
		_statusFont = new Gdiplus::Font(L"Segoe UI", 10);
		_pointFont = new Gdiplus::Font(L"Segoe UI", 8);
		_statusFontBrush = new Gdiplus::SolidBrush(Gdiplus::Color(128, 255, 255, 255));
		_pointFontBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 0, 0));
		_polyBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0, 0));

		thisFrame = 0;
		lastFrame = 0;
		frameCounter = 0;
		_totalFrames = 0;

		for(_index = 0; _index < 5; _index ++)
		{
			_fps[_index] = 0;
		}

		// Store the window height and width for reference later
		_screenWidth = viewportWidth;
		_screenHeight = viewportHeight;

		// Initialise scan line array
		_scanlines = new GDIScanline[viewportHeight];

		return 1; // debug
	}

	void RasteriserGDI::Clear(int alpha, int red, int green, int blue)
	{
		_graphics->Clear(Gdiplus::Color(alpha, red, green, blue));
	}

	void RasteriserGDI::Clear(Gdiplus::Color color)
	{
		_graphics->Clear(color);
	}

	void RasteriserGDI::DrawWindowInformation()
	{
		_textPoint.X = 12.0f;

		// Increase frame counter
		frameCounter++;

		// Update the counter
		thisFrame = GetTickCount();

		if(thisFrame - lastFrame > 1000) // If a second has elapsed...
		{
			// Move the previous values back in the arrays
			for(_index = 0; _index < 4; _index ++)
			{
				_fps[_index + 1] = _fps[_index];
			}

			// Store the current value
			_fps[0] = (float)frameCounter;

			// Reset the values
			frameCounter = 0;
			lastFrame = thisFrame;
		}

		// Increment the total frames counter
		_totalFrames++;

		// Draw the current FPS
		_textPoint.Y = 12.0f;
		swprintf(_textString, 60, L"%s %.0f %s %.1f%s", L"FPS:", _fps[0], L"(Last 5 seconds avg. FPS:", (_fps[0] + _fps[1] + _fps[2] + _fps[3] + _fps[4]) / 5, L")");
		_graphics->DrawString(_textString, -1, _statusFont, _textPoint, _statusFontBrush);

		// Draw the 5-second average FPS and total frames rendered
		_textPoint.Y = 26.0f;
		swprintf(_textString, 70, L"%s %u", L"Total frames rendered:", _totalFrames);
		_graphics->DrawString(_textString, -1, _statusFont, _textPoint, _statusFontBrush);

		// Draw the window size
		_textPoint.Y = 40.0f;
		swprintf(_textString, 40, L"%s %i %s %i %s %.3f", L"Window size:", _screenWidth, L"x", _screenHeight, L"Aspect:", ((float)_screenWidth / _screenHeight));
		_graphics->DrawString(_textString, -1, _statusFont, _textPoint, _statusFontBrush);
	}

	void RasteriserGDI::DrawCameraInformation(const Camera& cam)
	{		
		_textPoint.X = 12.0f;

		// Draw camera position
		_textPoint.Y = 60.0f;
		swprintf(_textString, 40, L"%s %.1f, %.1f, %.1f", L"CameraPos:", cam._position->_vector._x, cam._position->_vector._y, cam._position->_vector._z);
		_graphics->DrawString(_textString, -1, _statusFont, _textPoint, _statusFontBrush);

		// Draw camera rotation
		_textPoint.Y = 74.0f;
		swprintf(_textString, 40, L"%s %.2f, %.2f, %.2f", 
			L"CameraRot:", (cam._rotation->_vector._x / (M_PI / 180)), (cam._rotation->_vector._y / (M_PI / 180)), (cam._rotation->_vector._z/ (M_PI / 180)));
		_graphics->DrawString(_textString, -1, _statusFont, _textPoint, _statusFontBrush);
	}

	void RasteriserGDI::DrawVertTextPoints(Object& obj)
	{
		for (_index = 0; _index < obj.GetNumberOfVerts(); ++_index) // For each vert...
		{
			// Set the text point to the vert's location
			_textPoint.X = obj._screenSpaceVerts[_index]._vector._x;
			_textPoint.Y = obj._screenSpaceVerts[_index]._vector._y;

			// Create the string and draw
			swprintf(_textString, 40, L"%s %.1f %s %.1f %s %.1f", 
				L"X:", obj._worldSpaceVerts[_index]._vector._x, L"Y:", obj._worldSpaceVerts[_index]._vector._y, L"Z:", obj._worldSpaceVerts[_index]._vector._z);
			_graphics->DrawString(_textString, -1, _pointFont, _textPoint, _pointFontBrush);
		}
	}

	void RasteriserGDI::DrawRendererInformation(int modelCount, int lightCount, int vertCount)
	{
		_textPoint.X = 12.0f;
		_textPoint.Y = (float)_screenHeight - 86.0f;

		swprintf(_textString, 60, L"%s %u %s %u %s %u",
			L"Model count:", modelCount, // Model count
			L"- Light count:", lightCount, // Light count
			L"- Total verts:", vertCount);

		_graphics->DrawString(_textString, -1, _pointFont, _textPoint, _statusFontBrush);
	}

	void RasteriserGDI::DrawWireframe(Object& obj, enum E_DRAWTYPE_WIREFRAME eTransformArgs)
	{
		static const RenderLibrary::Polygon* poly; // Pointer to poly being rendered (avoids iteration)
		
		// Select the correct draw method
		switch(eTransformArgs)
		{
#pragma region Wireframe
		case 0: // WIREFRAME
			// Draw lines from X to Y, Y to Z, and Z to X for each poly in the object
			for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
			{
				poly = obj._polys[_index];
				
				// X -> Y
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
					obj._screenSpaceVerts[poly->_data[0]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
					obj._screenSpaceVerts[poly->_data[1]]._vector._y);

				// Y -> Z
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
					obj._screenSpaceVerts[poly->_data[1]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
					obj._screenSpaceVerts[poly->_data[2]]._vector._y);

				// Z -> X
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
					obj._screenSpaceVerts[poly->_data[2]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
					obj._screenSpaceVerts[poly->_data[0]]._vector._y);
			}
			break;
#pragma endregion
#pragma region Wireframe with Text Points
		case 1: // WIREFRAME_WITH_TEXT_POINTS
			for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
			{
				poly = obj._polys[_index];
				
				// X -> Y
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
					obj._screenSpaceVerts[poly->_data[0]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
					obj._screenSpaceVerts[poly->_data[1]]._vector._y);

				// Y -> Z
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
					obj._screenSpaceVerts[poly->_data[1]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
					obj._screenSpaceVerts[poly->_data[2]]._vector._y);

				// Z -> X
				_graphics->DrawLine(_pen, 
					obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
					obj._screenSpaceVerts[poly->_data[2]]._vector._y, 
					obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
					obj._screenSpaceVerts[poly->_data[0]]._vector._y);
			}

			// Draw the text on the points
			DrawVertTextPoints(obj);
			break;
#pragma endregion
#pragma region Wireframe + Backface Culling
		case 2: // WIREFRAME_BACKFACE
			// Draw lines from X to Y, Y to Z, and Z to X for each poly in the object
			for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
			{
				// Do the following if the poly isn't marked as back facing
				if (obj._polys[_index]->_backface == false)
				{
					poly = obj._polys[_index];
					
					// X -> Y
					//_graphics -> DrawLine(_pen, pX, pY);
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
						obj._screenSpaceVerts[poly->_data[0]]._vector._y, 
						obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
						obj._screenSpaceVerts[poly->_data[1]]._vector._y);

					// Y -> Z
					//_graphics -> DrawLine(_pen, pY, pZ);
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
						obj._screenSpaceVerts[poly->_data[1]]._vector._y, 
						obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
						obj._screenSpaceVerts[poly->_data[2]]._vector._y);

					// Z -> X
					//_graphics -> DrawLine(_pen, pZ, pX); 
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
						obj._screenSpaceVerts[poly->_data[2]]._vector._y, 
						obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
						obj._screenSpaceVerts[poly->_data[0]]._vector._y);
				}
			}
			break;
#pragma endregion
#pragma region Wireframe with Text Points + Backface
		case 3: // WIREFRAME_BACKFACE_WITH_TEXT_POINTS
			// Draw lines from X to Y, Y to Z, and Z to X for each poly in the object
			for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
			{
				// Do the following if the poly isn't marked as back facing
				if (obj._polys[_index]->_backface == false)
				{
					poly = obj._polys[_index];
					
					// X -> Y
					//_graphics -> DrawLine(_pen, pX, pY);
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
						obj._screenSpaceVerts[poly->_data[0]]._vector._y, 
						obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
						obj._screenSpaceVerts[poly->_data[1]]._vector._y);

					// Y -> Z
					//_graphics -> DrawLine(_pen, pY, pZ);
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
						obj._screenSpaceVerts[poly->_data[1]]._vector._y, 
						obj._screenSpaceVerts[obj._polys[_index]->_data[2]]._vector._x, // Z
						obj._screenSpaceVerts[obj._polys[_index]->_data[2]]._vector._y);

					// Z -> X
					//_graphics -> DrawLine(_pen, pZ, pX); 
					_graphics->DrawLine(_pen, 
						obj._screenSpaceVerts[obj._polys[_index]->_data[2]]._vector._x, // Z
						obj._screenSpaceVerts[obj._polys[_index]->_data[2]]._vector._y, 
						obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
						obj._screenSpaceVerts[poly->_data[0]]._vector._y);
				}
			}

			// Draw the text on the points
			DrawVertTextPoints(obj);
			break;
#pragma endregion
		}
	}

	void RasteriserGDI::DrawFilledPolygons(Object &obj)
	{
		static const RenderLibrary::Polygon* poly; // Pointer to poly being rendered (avoids iteration)
		
#pragma region Flat Shading
		for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
		{
			// Do the following if the poly isn't marked as back facing
			if (obj._polys[_index]->_backface == false)
			{
				poly = obj._polys[_index]; // Set the pointer for the poly

				// Create Points from all the locations in the poly
				_poly[0].X = obj._screenSpaceVerts[poly->_data[0]]._vector._x;
				_poly[0].Y = obj._screenSpaceVerts[poly->_data[0]]._vector._y;

				_poly[1].X = obj._screenSpaceVerts[poly->_data[1]]._vector._x;
				_poly[1].Y = obj._screenSpaceVerts[poly->_data[1]]._vector._y;

				_poly[2].X = obj._screenSpaceVerts[poly->_data[2]]._vector._x;
				_poly[2].Y = obj._screenSpaceVerts[poly->_data[2]]._vector._y;

				// Draw the filled polygons
				_polyBrush->SetColor(poly->_light);
				_graphics->FillPolygon(_polyBrush, _poly, 3);
			}

			// Expand this to allow GDI+ Gouraud shading
		}
#pragma endregion
	}

	void RasteriserGDI::DrawScanLines(Object &obj, enum E_DRAWTYPE_FILLEDPOLYGONS eTransformArgs)
	{		
		static float interpolationAmount = 0; // debug
		static float newValue = 0;
		static int pointY1 = 0;
		static int pointY2 = 0;
		static int pointX1 = 0;
		static int pointX2 = 0;

		static const Polygon* _poly; // Pointer to poly being rendered (avoids iteration)

		// Select the correct draw method
		switch (eTransformArgs)
		{
#pragma region Flat Shading
		case 0:	// FLATSHADING
			for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index) // For each poly:
			{
				if (obj._polys[_index]->_backface == false) // Do the following if the poly isn't marked as back facing:
				{
					_poly = obj._polys[_index]; // Set the pointer for the poly
					
					for (_index2 = 0; _index2 < _screenHeight; ++_index2) // Reset the scan line array
					{
						_scanlines[_index2]._maxX = 0;
						_scanlines[_index2]._minX = _screenWidth;
					}
					
					// Interpolate the vertices // n = n1 + t(n2 - n1) // For each edge of the polygon, interpolate in-between values of x

					// Edge 0 -> 1
					// Find the top and bottom points
					pointY1 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._y;
					pointY2 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._y;

					// Find the left and right extents
					pointX1 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._x;
					pointX2 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._x;

					// Find the relationship between the interpolation value and the number of scan lines
					if (pointY1 - pointY2 != 0) 
					{
						if (pointY1 > pointY2)
						{
							pointY1 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._y;
							pointY2 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._y;

							pointX1 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._x;
							pointX2 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._x;
						}
						
						interpolationAmount = (float)(pointX1 - pointX2) / (float)(pointY1 - pointY2);
						newValue = 0;

						for (_index2 = pointY1; _index2 < pointY2; _index2++) // Fill in the scan line array
						{
							newValue += interpolationAmount;
							
							// If _maxX is LESS than the adjusted interpolation value, reduce it
							if (_scanlines[_index2]._maxX < pointX1 + newValue)
								_scanlines[_index2]._maxX = (int)(pointX1 + newValue);

							// If _minX is MORE than the adjusted interpolation value, increase it
							if (_scanlines[_index2]._minX > pointX1 + newValue)
								_scanlines[_index2]._minX = (int)(pointX1 + newValue);
						}
					}

					// Edge 1 -> 2
					// Find the top and bottom points
					pointY1 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._y;
					pointY2 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._y;

					// Find the left and right extents
					pointX1 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._x;
					pointX2 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._x;

					// Find the relationship between the interpolation value and the number of scan lines
					if (pointY1 - pointY2 != 0)
					{
						if (pointY1 > pointY2)
						{
							pointY1 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._y;
							pointY2 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._y;

							pointX1 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._x;
							pointX2 = (int)obj._screenSpaceVerts[_poly->_data[1]]._vector._x;
						}
						
						interpolationAmount = (float)(pointX1 - pointX2) / (float)(pointY1 - pointY2);
						newValue = 0;

						for (_index2 = pointY1; _index2 < pointY2; _index2++) // Fill in the scan line array
						{
							newValue += interpolationAmount;

							// If _maxX is LESS than the adjusted interpolation value, reduce it
							if (_scanlines[_index2]._maxX < pointX1 + newValue)
								_scanlines[_index2]._maxX = (int)(pointX1 + newValue);

							// If _minX is MORE than the adjusted interpolation value, increase it
							if (_scanlines[_index2]._minX > pointX1 + newValue)
								_scanlines[_index2]._minX = (int)(pointX1 + newValue);
						}
					}

					// Edge 2 -> 0
					// Find the top and bottom points
					pointY1 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._y;
					pointY2 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._y;

					// Find the left and right extents
					pointX1 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._x;
					pointX2 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._x;

					// Find the relationship between the interpolation value and the number of scan lines
					if (pointY1 - pointY2 != 0)
					{
						if (pointY1 > pointY2)
						{
							pointY1 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._y;
							pointY2 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._y;

							pointX1 = (int)obj._screenSpaceVerts[_poly->_data[0]]._vector._x;
							pointX2 = (int)obj._screenSpaceVerts[_poly->_data[2]]._vector._x;
						}

						interpolationAmount = (float)(pointX1 - pointX2) / (float)(pointY1 - pointY2);
						newValue = 0;

						for (_index2 = pointY1; _index2 < pointY2; _index2++) // Fill in the scan line array
						{
							newValue += interpolationAmount;

							// If _maxX is LESS than the adjusted interpolation value, reduce it
							if (_scanlines[_index2]._maxX < pointX1 + newValue)
								_scanlines[_index2]._maxX = (int)(pointX1 + newValue);

							// If _minX is MORE than the adjusted interpolation value, increase it
							if (_scanlines[_index2]._minX > pointX1 + newValue)
								_scanlines[_index2]._minX = (int)(pointX1 + newValue);
						}
					}

					// Plot the pixels // _bitmap->SetPixel();
					for(_index2 = 0; _index2 < (int)_screenHeight; _index2++) // For each scan line...
					{
						for(_index3 = _scanlines[_index2]._minX; _index3 < _scanlines[_index2]._maxX; _index3++)
						{
							//if((_index3 > 0 && _index3 < _screenWidth) && (_index2 > 0 && _index2 < _screenWidth))
								_bitmap->SetPixel(_index3, _index2, obj._polys[_index]->_light);
						}
					}
				}	
			}
			break;
#pragma endregion
#pragma region Gouraud Shading
		case 1:	// GOURAUDSHADING
			// Do some cool shit here
			break;
#pragma endregion
		}
	}

	Gdiplus::Bitmap* RasteriserGDI::GetGDIplusBitmap()
	{
		return _bitmap;
	}
}
