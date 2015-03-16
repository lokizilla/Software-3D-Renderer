#include "stdafx.h"		// PCH

#define _USE_MATH_DEFINES
#include <math.h>		// Math functions

#include "Matrix.h"
#include "Camera.h"

using namespace MathLibrary;

namespace RenderLibrary
{
	Camera::Camera()
	{
		// Initialise() must be called!
		
		_worldTransform = NULL;
		_cameraTransform = NULL;
		_perspectiveTransform = NULL;
		_screenTransform = NULL;

		_temp1 = NULL;
		_temp2 = NULL;
		_temp3 = NULL;

		_position = NULL;
		_rotation = NULL;
	}

	Camera::~Camera()
	{
		// Delete member matrices
		delete _cameraTransform;
		delete _perspectiveTransform;
		delete _screenTransform;
	}

	int Camera::Initialise(int viewportWidth, int viewportHeight)
	{
		// Create new view and transform matrices
		if(_worldTransform == NULL)
			_worldTransform = new MathLibrary::Matrix();
		if(_cameraTransform == NULL)
			_cameraTransform = new MathLibrary::Matrix();
		if(_perspectiveTransform == NULL)
			_perspectiveTransform = new MathLibrary::Matrix();
		if(_screenTransform == NULL)
			_screenTransform = new MathLibrary::Matrix();

		if(_temp1 == NULL)
			_temp1 = new MathLibrary::Matrix();
		if(_temp2 == NULL)
			_temp2 = new MathLibrary::Matrix();
		if(_temp3 == NULL)
			_temp3 = new MathLibrary::Matrix();

		if(_position == NULL)
			_position = new Vector();
		if(_rotation == NULL)
			_rotation = new Vector();

		// Camera properties
		_fov = 9007604; // NOT USED ANYWHERE! FIX THIS!

		_nearDistance = 10.0f; // Near frustum
		_farDistance = 500.0f; // Far frustum
		_viewportWidth = viewportWidth; // Width of view port
		_viewportHeight = viewportHeight; // Height of view port

		return 1; // debug
	}
	
	// Build world transform
	void Camera::BuildWorldTransform(MathLibrary::Vector* rotation, MathLibrary::Vector* position)
	{
		// Clear the matrices
		_worldTransform->InitialiseIdentity();
		_temp1->InitialiseIdentity();
		_temp2->InitialiseIdentity();
		_temp3->InitialiseIdentity();

		// Calculate translation
		MathLibrary::Matrix translation 
			(1, 0, 0, position->_vector._x,
			0, 1, 0, position->_vector._y,
			0, 0, 1, position->_vector._z,
			0, 0, 0, 1);

		_worldTransform->Multiply(*_worldTransform, translation, *_temp1);

		// Calculate rotation matrices
		// X rotation (pitch)
		MathLibrary::Matrix xRotation 
			(1, 0, 0, 0,
			0, cos(rotation->_vector._x), -sin(rotation->_vector._x), 0,				
			0, sin(rotation->_vector._x), cos(rotation->_vector._x), 0,
			0, 0, 0, 1);

		_worldTransform->Multiply(*_temp1, xRotation, *_temp2);

		// Y rotation (yaw)
		MathLibrary::Matrix yRotation 
			(cos(rotation->_vector._y), 0, sin(rotation->_vector._y), 0,	
			0, 1, 0, 0,	
			-sin(rotation->_vector._y), 0, cos(rotation->_vector._y), 0,
			0, 0, 0, 1);

		_worldTransform->Multiply(*_temp2, yRotation, *_temp3);

		// Z rotation (roll)
		MathLibrary::Matrix zRotation 
			(cos(rotation->_vector._z), -sin(rotation->_vector._z), 0, 0,	
			sin(rotation->_vector._z), cos(rotation->_vector._z), 0, 0, 
			0, 0, 1, 0,	
			0, 0, 0, 1);

		_worldTransform->Multiply(*_temp3, zRotation, *_worldTransform);
	}

	// Build camera transform
	void Camera::BuildCameraTransform()
	{
		// [Leaving the camera at the origin, transform the objects around the camera]
		// Clear the matrices
		_cameraTransform->InitialiseIdentity();
		_temp1->InitialiseIdentity();
		_temp2->InitialiseIdentity();
		_temp3->InitialiseIdentity();

		// Calculate inverse rotation matrices
		// X rotation (pitch)
		MathLibrary::Matrix xRotation 
			(1, 0, 0, 0,
			0, cos(_rotation->_vector._x), sin(_rotation->_vector._x), 0,				
			0, -sin(_rotation->_vector._x), cos(_rotation->_vector._x), 0,
			0, 0, 0, 1);

		_cameraTransform->Multiply(*_cameraTransform, xRotation, *_temp1);

		// Y rotation (yaw)
		MathLibrary::Matrix yRotation 
			(cos(_rotation->_vector._y), 0, -sin(_rotation->_vector._y), 0,	
			0, 1, 0, 0,	
			sin(_rotation->_vector._y), 0, cos(_rotation->_vector._y), 0,
			0, 0, 0, 1);

		_cameraTransform->Multiply(*_temp1, yRotation, *_temp2);

		// Z rotation (roll)
		MathLibrary::Matrix zRotation 
			(cos(_rotation->_vector._z), sin(_rotation->_vector._z), 0, 0,	
			-sin(_rotation->_vector._z), cos(_rotation->_vector._z), 0, 0, 
			0, 0, 1, 0,	
			0, 0, 0, 1);

		_cameraTransform->Multiply(*_temp2, zRotation, *_temp3);

		// Calculate inverse translation
		MathLibrary::Matrix translation 
			(1, 0, 0, -_position->_vector._x,
			0, 1, 0, -_position->_vector._y,
			0, 0, 1, -_position->_vector._z,
			0, 0, 0, 1);

		_cameraTransform->Multiply(*_temp3, translation, *_cameraTransform);

		//_cameraTransform->Initialise(
		//	(cos(_rotation->_vector._y) * cos(_rotation->_vector._z)) + (sin(_rotation->_vector._y) * sin(_rotation->_vector._x) * sin(_rotation->_vector._z)),
		//	cos(_rotation->_vector._x) * sin(_rotation->_vector._z),
		//	(-(sin(_rotation->_vector._y)) * cos(_rotation->_vector._z)) + (cos(_rotation->_vector._y) * sin(_rotation->_vector._x) * sin(_rotation->_vector._z)),
		//	-(_position->_vector._x * _cameraTransform->_matrix._data[0][0]) + (_position->_vector._y * _cameraTransform->_matrix._data[0][1]) + (_position->_vector._z * _cameraTransform->_matrix._data[0][2]),

		//	(-(cos(_rotation->_vector._y)) * sin(_rotation->_vector._z)) + (sin(_rotation->_vector._y) * sin(_rotation->_vector._x) * cos(_rotation->_vector._z)),
		//	cos(_rotation->_vector._x) * cos(_rotation->_vector._z),
		//	(sin(_rotation->_vector._y) * sin(_rotation->_vector._z)) + (cos(_rotation->_vector._y) * sin(_rotation->_vector._x) * cos(_rotation->_vector._z)),
		//	-(_position->_vector._x * _cameraTransform->_matrix._data[1][0]) + (_position->_vector._y * _cameraTransform->_matrix._data[1][1]) + (_position->_vector._y * _cameraTransform->_matrix._data[1][2]),

		//	sin(_rotation->_vector._y) * cos(_rotation->_vector._x),
		//	-sin(_rotation->_vector._x),
		//	cos(_rotation->_vector._y) * cos(_rotation->_vector._x),
		//	-(_position->_vector._x * _cameraTransform->_matrix._data[2][0]) + (_position->_vector._y * _cameraTransform->_matrix._data[2][1]) + (_position->_vector._z * _cameraTransform->_matrix._data[2][2]),

		//	0, 0, 0, 1);
	}

	void Camera::BuildPerspectiveTransform()
	{
		// Clear the perspectiveTransform matrix
		_perspectiveTransform->InitialiseNull();

		// Create the projection matrix
		_perspectiveTransform->_matrix._data[0][0] = 1;
		_perspectiveTransform->_matrix._data[1][1] = ((float)_viewportWidth / _viewportHeight); // Aspect ratio correction
		_perspectiveTransform->_matrix._data[3][2] = 1;
	}

	void Camera::BuildScreenTransform()
	{
		// Clear the screenTransform matrix
		_screenTransform->InitialiseIdentity();

		// Calculate values
		float width = (float)_viewportWidth / 2;
		float height = (float)_viewportHeight / 2;

		// Create the screen space matrix (a, 0, 0, a, 0, -b, 0, b, 0, 0, 1, 0, 0, 0, 0, 1)
		_screenTransform->_matrix._data[0][0] = width - 0.5f;
		_screenTransform->_matrix._data[0][3] = width - 0.5f;
		_screenTransform->_matrix._data[1][1] = -height - 0.5f;
		_screenTransform->_matrix._data[1][3] = height - 0.5f;
		_screenTransform->_matrix._data[2][2] = 1;
		_screenTransform->_matrix._data[3][3] = 1;
	}
}