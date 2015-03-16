#include "StdAfx.h"
#include "RasteriserD2D.h"

namespace RenderLibrary
{
	RasteriserD2D::RasteriserD2D()
	{
		// Initialise() must be called

		// Use HeapSetInformation to specify that the process should
		// terminate if the heap manager detects an error in any heap used
		// by the process.
		// The return value is ignored, because we want to continue running in the
		// unlikely event that HeapSetInformation fails.
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

		_colourBrush = NULL;
		_direct2DFactory = NULL;
		_hWnd = NULL;
		_renderTarget = NULL;
	}

	RasteriserD2D::~RasteriserD2D()
	{
		
	}

	HRESULT RasteriserD2D::Initialise(HWND* hWnd)
	{
		_hWnd = hWnd; // Store the current HWND pointer
		
		HRESULT hr;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2DFactory); // Create a Direct2D factory
		
		return hr;
	}

	HRESULT RasteriserD2D::Shutdown()
	{
		//// Safe release the Direct2D objects
		//if (*_direct2dFactory != NULL)
		//{
		//	(*_direct2dFactory)->Release();
		//	(*_direct2dFactory) = NULL;
		//}
		//
		return S_OK; // Debug
	}

	HRESULT RasteriserD2D::CreateResources()
	{
		HRESULT hr = S_OK;

		if (!_renderTarget) // If RenderTarget does not exist:
		{
			RECT rc;
			GetClientRect(*_hWnd, &rc); // Get the window size

			D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top); // Set this into a D2D container

			hr = _direct2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(*_hWnd, size), &_renderTarget); // Create a Direct2D render target

			if (SUCCEEDED(hr))
				hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &_colourBrush); // Create a gray brush
		}

		return hr;
	}

	void RasteriserD2D::BeginDraw()
	{
		_renderTarget->BeginDraw();	
	}

	HRESULT RasteriserD2D::EndDraw()
	{
		HRESULT hr;
		
		hr = _renderTarget->EndDraw();

		if(hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;

			//if (*_renderTarget != NULL)
			//{
			//	*_renderTarget->Release();
			//	*_renderTarget = NULL;
			//}
		}

		return hr;
	}

	void RasteriserD2D::Clear()
	{
		_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black)); // Debug: clear to black
	}

	void RasteriserD2D::DrawWireframe(RenderLibrary::Object& obj, E_DRAWTYPE_FILLEDPOLYGONS eTransformArgs)
	{
		//_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		static const RenderLibrary::Polygon* poly; // Pointer to poly being rendered (avoids iteration)

		static int _index; 

		static D2D1_POINT_2F _point1;
		static D2D1_POINT_2F _point2;

		for (_index = 0; _index < obj.GetNumberOfPolys(); ++_index)
		{
			// Do the following if the poly isn't marked as back facing
			if (obj._polys[_index]->_backface == false)
			{
				poly = obj._polys[_index];

				// X -> Y
				//_graphics -> DrawLine(_pen, pX, pY);
				//_graphics->DrawLine(_pen, 
					//obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
					//obj._screenSpaceVerts[poly->_data[0]]._vector._y, 
					//obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
					//obj._screenSpaceVerts[poly->_data[1]]._vector._y);

				_point1.x = obj._screenSpaceVerts[poly->_data[0]]._vector._x;
				_point1.y = obj._screenSpaceVerts[poly->_data[0]]._vector._y;
				_point2.x = obj._screenSpaceVerts[poly->_data[1]]._vector._x;
				_point2.y = obj._screenSpaceVerts[poly->_data[1]]._vector._y;

				_renderTarget->DrawLine(_point1, _point2, _colourBrush, 1.0f, 0);

				// Y -> Z
				//_graphics -> DrawLine(_pen, pY, pZ);
				//_graphics->DrawLine(_pen, 
				//	obj._screenSpaceVerts[poly->_data[1]]._vector._x, // Y
				//	obj._screenSpaceVerts[poly->_data[1]]._vector._y, 
				//	obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
				//	obj._screenSpaceVerts[poly->_data[2]]._vector._y);

				_point1.x = obj._screenSpaceVerts[poly->_data[1]]._vector._x;
				_point1.y = obj._screenSpaceVerts[poly->_data[1]]._vector._y;
				_point2.x = obj._screenSpaceVerts[poly->_data[2]]._vector._x;
				_point2.y = obj._screenSpaceVerts[poly->_data[2]]._vector._y;

				_renderTarget->DrawLine(_point1, _point2, _colourBrush, 1.0f, 0);

				// Z -> X
				//_graphics -> DrawLine(_pen, pZ, pX); 
				//_graphics->DrawLine(_pen, 
				//	obj._screenSpaceVerts[poly->_data[2]]._vector._x, // Z
				//	obj._screenSpaceVerts[poly->_data[2]]._vector._y, 
				//	obj._screenSpaceVerts[poly->_data[0]]._vector._x, // X
				//	obj._screenSpaceVerts[poly->_data[0]]._vector._y);

				_point1.x = obj._screenSpaceVerts[poly->_data[2]]._vector._x;
				_point1.y = obj._screenSpaceVerts[poly->_data[2]]._vector._y;
				_point2.x = obj._screenSpaceVerts[poly->_data[0]]._vector._x;
				_point2.y = obj._screenSpaceVerts[poly->_data[0]]._vector._y;

				_renderTarget->DrawLine(_point1, _point2, _colourBrush, 1.0f, 0);
			}
		}
	}
}
