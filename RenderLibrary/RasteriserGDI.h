// Rasteriser header file.

#ifndef RASTERISERGDI_H
#define RASTERISERGDI_H

#include <GdiPlus.h>
#include "Object.h"
#include "ObjectManager.h"
#include "GDIScanline.h"

enum E_DRAWTYPE_WIREFRAME{WIREFRAME, WIREFRAME_WITH_TEXT_POINTS, WIREFRAME_BACKFACE, WIREFRAME_BACKFACE_WITH_TEXT_POINTS};
enum E_DRAWTYPE_FILLEDPOLYGONS{FLATSHADING, GOURAUDSHADING};

namespace RenderLibrary
{
	class RasteriserGDI
	{
	public:
		RasteriserGDI(); // Default constructor (Initialise() must be called!)
		~RasteriserGDI(); // Destructor

		int Initialise(int viewportWidth, int viewportHeight); // Set up the rasteriser

		// Interface for Gdiplus::Clear
		void Clear(int alpha, int red, int green, int blue); // Clear the window to the specified ARGB value
		void Clear(Gdiplus::Color color); // Clear the window to the specified colour

		// Text drawing
		void DrawWindowInformation();
		void DrawCameraInformation(const Camera& cam);
		void DrawVertTextPoints(Object& obj);
		void DrawRendererInformation(int modelCount, int lightCount, int vertCount);

		// Model drawing
		void DrawWireframe(Object& obj, enum E_DRAWTYPE_WIREFRAME eTransformArgs); // Draw the next frame in wire frame
		void DrawFilledPolygons(Object& obj); // Draw the next frame with filled polygons using GDI methods
		void DrawScanLines(Object& obj, enum E_DRAWTYPE_FILLEDPOLYGONS eTransformArgs); // Draw the next frame with filled polygons using per-pixel methods

		Gdiplus::Bitmap* GetGDIplusBitmap(); // Return a pointer to the bitmap

	private:
		int _screenHeight;
		int _screenWidth;

		Gdiplus::Bitmap* _bitmap;
		Gdiplus::Graphics* _graphics;
		
		Gdiplus::PointF _poly[3]; // PointF array for the FillPolygon method
		
		int	_index;
		int _index2;
		int _index3; // Used for loops

		int	thisFrame;
		int	lastFrame; // Store tick values from last frame to the current frame
		int	frameCounter; // Store the total number of frames rendered in that time
		int _totalFrames; // Total number of frames rendered
		float _fps[5]; // Store the previous ten FPS values

		Gdiplus::Pen* _pen; // Pen for line drawing

		// Text Objects
		Gdiplus::SolidBrush* _statusFontBrush; // Brush with which to draw font
		Gdiplus::Font* _statusFont; // Font for use with status messages in-window
		Gdiplus::SolidBrush* _pointFontBrush; // Brush with which to draw font
		Gdiplus::Font* _pointFont; // Font for use rendering next to model points
		Gdiplus::PointF _textPoint; // Point at which the FPS is drawn
		wchar_t _textString[70]; // Store any value to be written to screen

		Gdiplus::SolidBrush* _polyBrush; // Brush with which to draw polygons

		GDIScanline* _scanlines;
	};
}

#endif // RASTERISERGDI	_H