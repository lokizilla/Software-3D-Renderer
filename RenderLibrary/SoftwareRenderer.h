#ifndef SOFTWARERENDERER_H
#define SOFTWARERENDERER_H

#include "RasteriserGDI.h"
#include "Camera.h"
#include "Object.h"
#include "MD2Loader.h"
#include "ObjectManager.h"
#include "LightManager.h"
#include "LightDirectional.h"

namespace RenderLibrary
{
	class SoftwareRenderer
	{
	public:
		enum E_MANIPULATION_TYPE{M_ROTATE, M_TRANSLATE, M_COLOUR, M_SCALE};
		enum E_AXIS{AXIS_X, AXIS_Y, AXIS_Z};
		enum E_RENDERMODE{R_GDIPLUS, R_WIRE_FRAME, R_BACKFACE, R_TEXT_POINTS, R_FLAT_SHADING, R_GOURAUD, R_SCANLINE};

		SoftwareRenderer();
		~SoftwareRenderer();

		// Setup
		void Initialise(HWND* renderHWND, int hwndWidth, int hwndHeight); // Calls initialisation logic
		void Shutdown(); // Delete member objects

		// Functionality
		void Update(); // Calls update logic
		void Render(); // Calls render logic
		
		// Utilities
		void WindowSizeChanged(HWND* renderHWND); // Deal with a change of window size

		// Control
		void AddModel(const char* filename); // Load an object from the supplied filename into the object
		void AddDirectionalLight(MathLibrary::Vector direction, RenderLibrary::Light::LightColour colour); // Add a directional light
		void RemoveLastModel(); // Remove the last-added model
		void RemoveLastLight(); // Remove the last-added light
		void ChangeRenderMode(enum E_RENDERMODE renderMode, bool onOff); // Turn a render mode flag on or off

		void ManipulateObjectRelative(enum E_MANIPULATION_TYPE operationType, int objectNumber, enum E_AXIS axis, float amount); // Manipulate an object relative
		void ManipulateObjectDirect(enum E_MANIPULATION_TYPE operationType, int objectNumber, enum E_AXIS axis, float amount); // Manipulate an object directly
		void ManipulateObjectDirect(enum E_MANIPULATION_TYPE operationType, int objectNumber, Gdiplus::Color color); // Change an object's colour

		void ManipulateLightRelative(enum E_MANIPULATION_TYPE operationType, int lightNumber, enum E_AXIS axis, float amount); // Manipulate a light relative
		void ManipulateLightDirect(enum E_MANIPULATION_TYPE operationType, int lightNumber, enum E_AXIS axis, float amount); // Manipulate a light directly
		void ManipulateLightDirect(enum E_MANIPULATION_TYPE operationType, int lightNumber, RenderLibrary::Light::LightColour); // Change a light's colour

		void ManipulateCameraRelative(enum E_MANIPULATION_TYPE operationType, enum E_AXIS axis, float amount); // Manipulate the camera relative
		void ManipulateCameraDirect(enum E_MANIPULATION_TYPE operationType, enum E_AXIS axis, float amount); // Manipulate the camera directly

		// Flags
		bool F_GDIPLUS, F_WIREFRAME, F_BACKFACE, F_TEXT_POINTS, F_FLATSHADING, F_GOURAUD, F_SCANLINE; // F_ used for flag identification

		// 'Get' methods
		int GetNumberOFLights(); // Return the number of lights
		int GetNumberOfObjects(); // Return the number of objects

	private:
		int _numberOfLights;
		int _numberOfObjects;

		int _screenHeight;
		int _screenWidth;

		HWND* _currentHWND; // Pointer to the current HWND
		
		Gdiplus::Graphics* _screen; // GDI+ interface: required to draw the image to the screen

		RenderLibrary::RasteriserGDI* _rasteriserGDI; // GDI+ Rasteriser: takes care of the rendering work
		RenderLibrary::Camera* _camera; // Camera: flattens the scene into a 2D image
		RenderLibrary::ObjectManager* _objectManager; // ObjectManager: store a group of multiple objects
		RenderLibrary::Light::LightManager* _lightManager; // LightManager: store a group of multiple lights

		unsigned int index;
	};
}

#endif // SOFTWARERENDERER_H
