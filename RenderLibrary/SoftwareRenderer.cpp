#include "StdAfx.h"
#include "SoftwareRenderer.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm> // Contains definitions for the sort

#include "ObjectManager.h"
#include "LightManager.h"
#include "BaseLight.h"
#include "LightAmbient.h"
#include "LightDirectional.h"

namespace RenderLibrary
{
	static float _rotation = 0.0f;
	static float _rotAmount = 0.0075f;

	bool SortDepth(RenderLibrary::Object* object1, RenderLibrary::Object* object2) // Binary predicate: sort by average depth for all objects in manager
	{	
		return object1->_averageDepth < object2->_averageDepth;
	}

	SoftwareRenderer::SoftwareRenderer()
	{
		_rasteriserGDI = NULL;
		_screen = NULL;
		
		_numberOfObjects = 0;
		_numberOfLights = 1;

		_objectManager = new ObjectManager();
		_objectManager->Initialise(); // Create and initialise the ObjectManager

		_lightManager = new RenderLibrary::Light::LightManager();
		_lightManager->Initialise(); // Create and initialise the LightManager

		_camera = new Camera(); // Create the camera

		// Debug: Push some sample lights
		_lightManager->_lights.push_back(new RenderLibrary::Light::LightAmbient());
		_lightManager->_lights[0]->Initialise(RenderLibrary::Light::LightColour(25, 25, 25, 255));

		AddDirectionalLight(MathLibrary::Vector(-210, -210, -210), RenderLibrary::Light::LightColour(0, 0, 255, 255));

		AddDirectionalLight(MathLibrary::Vector(210, 210, 210), RenderLibrary::Light::LightColour(0, 255, 0, 255));

		// Set all mode flags to false by default
		F_GDIPLUS = false;
		F_WIREFRAME = false;
		F_BACKFACE = false;
		F_TEXT_POINTS = false;
		F_FLATSHADING = false;
		F_GOURAUD = false;
		F_SCANLINE = false;
	}

	SoftwareRenderer::~SoftwareRenderer()
	{
		// Shutdown() must be called
	}

	void SoftwareRenderer::Initialise(HWND* renderHWND, int hwndWidth, int hwndHeight)
	{
		_screenWidth = hwndWidth;
		_screenHeight = hwndHeight;
		_currentHWND = renderHWND;
		
		_camera->Initialise(hwndWidth, hwndHeight); // Initialise the camera

		// Create transforms based on screen size / aspect ratio which stay the same
		_camera->BuildPerspectiveTransform(); // Build the Perspective transform
		_camera->BuildScreenTransform(); // Build the Screen transform
	}

	void SoftwareRenderer::Shutdown()
	{
		// Delete members
		if(_rasteriserGDI != NULL)
			delete _rasteriserGDI;

		delete _camera;
		delete _objectManager;
		delete _lightManager;
		
		if(_screen != NULL)
			delete _screen;
	}

	void SoftwareRenderer::Update()
	{
		RenderLibrary::Object* object; // Store a pointer to an object

		_rotation += _rotAmount; // Change the rotation value by the predefined amount

		_camera->_position->_vector._x = 0.0f;
		_camera->_rotation->_vector._x = 0.0f;
		_camera->_position->_vector._y = 0.0f;
		_camera->_rotation->_vector._y = 0.0f;
		_camera->_position->_vector._z = -800.0f;
		_camera->_rotation->_vector._z = 0.0f;

		// Move the camera around the scene
		_camera->_rotation->_vector._x = -(sin(_rotation) * 0.6f);
		_camera->_position->_vector._x = sin(_rotation * 2.0f) * 500.0f;
		_camera->_rotation->_vector._y = sin(_rotation * 2.0f) * 0.5f;
		_camera->_position->_vector._y = sin(_rotation) * 600.0f;
		_camera->_rotation->_vector._z += sin(_rotation * 2.0f) * 0.45f;
		_camera->_position->_vector._z = sin(_rotation * 2.0f) * 400.0f + 3500;

		// Clamp the value at pi x 2 (6.28318530717958647692)
		if (_rotation >= 6.28318530717958647692) 
			_rotation = 0;

		_camera->BuildCameraTransform(); // Build the Camera transform

		for (index = 0; index < _objectManager->_objects.size(); index++) // For all objects in the scene...
		{
			object = _objectManager->_objects[index];

			_camera->BuildWorldTransform(object->_rotation, object->_position); // Build a new world-space transform
			object->Transform(*_camera->_worldTransform, WORLD_SPACE_VERTS); // Apply the world-space transform

			object->CalculateBackfaces(*_camera); // Calculate back face culling

			object->Transform(*_camera->_cameraTransform, CAMERA_SPACE_VERTS); // Apply the camera-space transform

			object->DepthSort(); // Depth-sort the object

			object->Transform(*_camera->_perspectiveTransform, PROJECTION_SPACE_VERTS); // Apply the projection-space transform

			_lightManager->ProcessLights(object); // Calculate lighting

			object->DeHomogenise(); // De-homogenise the verts

			object->Transform(*_camera->_screenTransform, SCREEN_SPACE_VERTS); // Apply the screen-space transform
		}

		std::sort(_objectManager->_objects.begin(), _objectManager->_objects.end(), SortDepth); // Depth-sort the ObjectManager
	}

	void SoftwareRenderer::Render()
	{
		if (F_GDIPLUS == true) // If GDI+ is on:
		{
			_rasteriserGDI->Clear(Gdiplus::Color::Black); // Clear the image // Optimisation: lock, clear (memset), unlock the buffer

			for (index = 0; index < _objectManager->_objects.size(); index++) // For all objects in the scene...
			{ 
				// Filled polygons via per-pixel methods using GDI+
				if (F_SCANLINE == true)
				{
					if (F_FLATSHADING == true && F_GOURAUD == false) // Scan lines, flat shading // ASSUMES F_BACKFACE
						_rasteriserGDI->DrawScanLines(*_objectManager->_objects[index], FLATSHADING); 

					else if (F_FLATSHADING == false && F_GOURAUD == true) // Scan lines, Gouraud shading // ASSUMES F_BACKFACE
						_rasteriserGDI->DrawScanLines(*_objectManager->_objects[index], GOURAUDSHADING); 
				}

				if (F_SCANLINE == false)
				{
					// Filled polygons using GDI+
					if (F_FLATSHADING == true)
						_rasteriserGDI->DrawFilledPolygons(*_objectManager->_objects[index]); // Draw with flat shading // ASSUMES F_BACKFACE

					// Add functionality to do GDI rendering of Gouraud shading
				}	

				// Wire frame using GDI+
				if (F_WIREFRAME == true)
				{
					if (F_BACKFACE == false && F_TEXT_POINTS == false) // Wire frame, no back facing, no text points
						_rasteriserGDI->DrawWireframe(*_objectManager->_objects[index], WIREFRAME);

					else if (F_BACKFACE == false && F_TEXT_POINTS == true) // Wire frame, no back facing, text points
						_rasteriserGDI->DrawWireframe(*_objectManager->_objects[index], WIREFRAME_WITH_TEXT_POINTS);

					else if (F_BACKFACE == true && F_TEXT_POINTS == false) // Wire frame, back facing, no text points
						_rasteriserGDI->DrawWireframe(*_objectManager->_objects[index], WIREFRAME_BACKFACE);

					else if (F_BACKFACE == true && F_TEXT_POINTS == true) // Wire frame, back facing, text points
						_rasteriserGDI->DrawWireframe(*_objectManager->_objects[index], WIREFRAME_BACKFACE_WITH_TEXT_POINTS);		
				}		
			}

			_rasteriserGDI->DrawWindowInformation(); // Draw window information (size, FPS)
			_rasteriserGDI->DrawCameraInformation(*_camera); // Draw some camera information
			_rasteriserGDI->DrawRendererInformation(_objectManager->_totalObjects, _lightManager->_lights.size(), _objectManager->_totalVerts); // Draw some renderer information

			_screen->DrawImage(_rasteriserGDI->GetGDIplusBitmap(), 0, 0); // Render the frame to the screen
		}
	}

	void SoftwareRenderer::WindowSizeChanged(HWND* renderHWND)
	{
		// Delete existing renderer objects that depend on window size
		delete _screen;
		if(_rasteriserGDI != NULL)
			delete _rasteriserGDI;

		// Get the new window size
		RECT _windowSize;
		GetWindowRect(*_currentHWND, &_windowSize);

		// Preserve the current camera position / orientation
		static MathLibrary::Vector camPos;
		static MathLibrary::Vector camOri;

		camPos._vector._x = _camera->_position->_vector._x;
		camPos._vector._y = _camera->_position->_vector._y;
		camPos._vector._z = _camera->_position->_vector._z;
		camOri._vector._x = _camera->_position->_vector._x;
		camOri._vector._y = _camera->_position->_vector._y;
		camOri._vector._z = _camera->_position->_vector._z;

		// Re-create these objects
		_screenHeight = _windowSize.bottom - _windowSize.top;
		_screenWidth = _windowSize.right - _windowSize.left;
		Initialise(renderHWND, _screenWidth, _screenHeight);

		if(F_GDIPLUS == true)
		{
			_rasteriserGDI = new RasteriserGDI(); // Create the rasteriser
			_rasteriserGDI->Initialise(_screenWidth, _screenHeight); // Initialise the rasteriser

			_screen = new Gdiplus::Graphics(*_currentHWND); // Create a new Graphics object
		}

		// Restore the camera position / orientation
		_camera->_position->_vector._x = camPos._vector._x;
		_camera->_position->_vector._y = camPos._vector._y;
		_camera->_position->_vector._z = camPos._vector._z;
		_camera->_position->_vector._x = camOri._vector._x;
		_camera->_position->_vector._y = camOri._vector._y;
		_camera->_position->_vector._z = camOri._vector._z;
	}

	void SoftwareRenderer::AddModel(const char* filename)
	{
		_objectManager->AddObject(filename);

		_numberOfObjects++;
	}

	void SoftwareRenderer::AddDirectionalLight(MathLibrary::Vector direction, RenderLibrary::Light::LightColour colour)
	{
		_lightManager->AddLight(new RenderLibrary::Light::LightDirectional());

		_lightManager->_lights.back()->Initialise(colour);

		_lightManager->_lights.back()->_direction->_vector._x = direction._vector._x;
		_lightManager->_lights.back()->_direction->_vector._y = direction._vector._y;
		_lightManager->_lights.back()->_direction->_vector._z = direction._vector._z;

		_numberOfLights++;
	}

	void SoftwareRenderer::RemoveLastModel()
	{
		if (_objectManager->_objects.size() != 0)
		{
			delete _objectManager->_objects.back();
			_objectManager->_objects.pop_back();

			_numberOfObjects--;
		}
	}

	void SoftwareRenderer::RemoveLastLight()
	{
		if (_lightManager->_lights.size() != 0)
		{
			delete _lightManager->_lights.back();
			_lightManager->_lights.pop_back();

			_numberOfLights--;
		}
	}

	void SoftwareRenderer::ChangeRenderMode(enum E_RENDERMODE renderMode, bool onOff)
	{
		if(renderMode == R_GDIPLUS)
		{
			F_GDIPLUS = onOff;

			if(onOff == true) // Switch to GDI+
			{
				_rasteriserGDI = new RenderLibrary::RasteriserGDI();
				_rasteriserGDI->Initialise(_screenWidth, _screenHeight);
				_screen = new Gdiplus::Graphics(*_currentHWND);
			}
		}

		else if(renderMode == R_WIRE_FRAME)
			F_WIREFRAME = onOff;

		else if(renderMode == R_BACKFACE)
			F_BACKFACE = onOff;

		else if(renderMode == R_TEXT_POINTS)
			F_TEXT_POINTS = onOff;

		else if(renderMode == R_FLAT_SHADING)
			F_FLATSHADING = onOff;

		else if(renderMode == R_GOURAUD)
			F_GOURAUD = onOff;

		else if(renderMode == R_SCANLINE)
			F_SCANLINE = onOff;
	}

	void SoftwareRenderer::ManipulateObjectRelative(enum E_MANIPULATION_TYPE operationType, int objectNumber, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_objectManager->_objects[objectNumber]->_rotation->_vector._x += amount;
			else if (axis == AXIS_Y)
				_objectManager->_objects[objectNumber]->_rotation->_vector._y += amount;
			else if (axis == AXIS_Z)
				_objectManager->_objects[objectNumber]->_rotation->_vector._z += amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_objectManager->_objects[objectNumber]->_position->_vector._x += amount;
			else if (axis == AXIS_Y)
				_objectManager->_objects[objectNumber]->_position->_vector._y += amount;
			else if (axis == AXIS_Z)
				_objectManager->_objects[objectNumber]->_position->_vector._z += amount;
		}

		else if (operationType == M_SCALE)
		{
			// Not currently implemented
		}
	}

	void SoftwareRenderer::ManipulateObjectDirect(enum E_MANIPULATION_TYPE operationType, int objectNumber, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_objectManager->_objects[objectNumber]->_rotation->_vector._x = amount;
			else if (axis == AXIS_Y)
				_objectManager->_objects[objectNumber]->_rotation->_vector._y = amount;
			else if (axis == AXIS_Z)
				_objectManager->_objects[objectNumber]->_rotation->_vector._z = amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_objectManager->_objects[objectNumber]->_position->_vector._x = amount;
			else if (axis == AXIS_Y)
				_objectManager->_objects[objectNumber]->_position->_vector._y = amount;
			else if (axis == AXIS_Z)
				_objectManager->_objects[objectNumber]->_position->_vector._z = amount;
		}

		else if (operationType == M_SCALE)
		{
			// Not currently implemented
		}
	}

	void SoftwareRenderer::ManipulateLightRelative(enum E_MANIPULATION_TYPE operationType, int lightNumber, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_lightManager->_lights[lightNumber]->_direction->_vector._x += amount;
			else if (axis == AXIS_Y)
				_lightManager->_lights[lightNumber]->_direction->_vector._y += amount;
			else if (axis == AXIS_Z)
				_lightManager->_lights[lightNumber]->_direction->_vector._z += amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_lightManager->_lights[lightNumber]->_position->_vector._x += amount;
			else if (axis == AXIS_Y)
				_lightManager->_lights[lightNumber]->_position->_vector._y += amount;
			else if (axis == AXIS_Z)
				_lightManager->_lights[lightNumber]->_position->_vector._z += amount;
		}
	}

	void SoftwareRenderer::ManipulateLightDirect(enum E_MANIPULATION_TYPE operationType, int lightNumber, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_lightManager->_lights[lightNumber]->_direction->_vector._x = amount;
			else if (axis == AXIS_Y)
				_lightManager->_lights[lightNumber]->_direction->_vector._y = amount;
			else if (axis == AXIS_Z)
				_lightManager->_lights[lightNumber]->_direction->_vector._z = amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_lightManager->_lights[lightNumber]->_position->_vector._x = amount;
			else if (axis == AXIS_Y)
				_lightManager->_lights[lightNumber]->_position->_vector._y = amount;
			else if (axis == AXIS_Z)
				_lightManager->_lights[lightNumber]->_position->_vector._z = amount;
		}
	}

	void SoftwareRenderer::ManipulateCameraRelative(enum E_MANIPULATION_TYPE operationType, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_camera->_rotation->_vector._x += amount;
			else if (axis == AXIS_Y)
				_camera->_rotation->_vector._y += amount;
			else if (axis == AXIS_Z)
				_camera->_rotation->_vector._z += amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_camera->_position->_vector._x += amount;
			else if (axis == AXIS_Y)
				_camera->_position->_vector._y += amount;
			else if (axis == AXIS_Z)
				_camera->_position->_vector._z += amount;
		}

		else if (operationType == M_SCALE)
		{
			// Not currently implemented
		}
	}

	void SoftwareRenderer::ManipulateCameraDirect(enum E_MANIPULATION_TYPE operationType, enum E_AXIS axis, float amount)
	{
		if (operationType == M_ROTATE)
		{
			if (axis == AXIS_X)
				_camera->_rotation->_vector._x = amount;
			else if (axis == AXIS_Y)
				_camera->_rotation->_vector._y = amount;
			else if (axis == AXIS_Z)
				_camera->_rotation->_vector._z = amount;
		}

		else if (operationType == M_TRANSLATE)
		{		
			if (axis == AXIS_X)
				_camera->_position->_vector._x = amount;
			else if (axis == AXIS_Y)
				_camera->_position->_vector._y = amount;
			else if (axis == AXIS_Z)
				_camera->_position->_vector._z = amount;
		}

		else if (operationType == M_SCALE)
		{
			// Not currently implemented
		}
	}

	int SoftwareRenderer::GetNumberOFLights()
	{
		return _numberOfLights;
	}

	int SoftwareRenderer::GetNumberOfObjects()
	{
		return _numberOfObjects;
	}
}
