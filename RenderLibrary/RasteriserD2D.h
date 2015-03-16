#ifndef RASTERISERD2D_H
#define RASTERISERD2D_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

namespace RenderLibrary
{
	class RasteriserD2D
	{
	public:
		RasteriserD2D(); // Default constructor (Initialise() must be called)
		~RasteriserD2D(); // Destructor

		HRESULT Initialise(HWND* hWnd); // Initialise the rasteriser (assumes RasteriserGDI has been destroyed first)
		HRESULT Shutdown(); // Safely free the rasteriser

		HRESULT CreateResources(); // Create resources used by the D3DDevice (quick-dropout loop used on call to render to ensure D3DDevice has not been lost)

		void BeginDraw();
		HRESULT EndDraw();

		void Clear(); // Clear the frame
		void DrawWireframe(RenderLibrary::Object& obj, E_DRAWTYPE_FILLEDPOLYGONS eTransformArgs); // Draw the next frame with filled polygons

		HWND* _hWnd; // Pointer to the current HWND 
		ID2D1Factory* _direct2DFactory;
		ID2D1HwndRenderTarget* _renderTarget;
		ID2D1SolidColorBrush* _colourBrush;
	};
}

#endif // RASTERISERD2D_H
