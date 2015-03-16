#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"

namespace RenderLibrary
{
	class Camera
	{
	public:
		// Constructors
		Camera(); // Default constructor
		~Camera(); // Destructor
		//const Camera(const Camera& cam); // Copy constructor

		// Utilities
		int	Initialise(int width, int height); // Initialise the camera

		// Transforms
		void BuildWorldTransform(MathLibrary::Vector* rotation, MathLibrary::Vector* position); // Transform to world space using object's rotation and translation values
		void BuildCameraTransform(); // Transform to camera (user) space using rotation and translation values
		void BuildPerspectiveTransform(); // Transform to projection space
		void BuildScreenTransform(); // Transform to screen space

		// Members
		MathLibrary::Matrix* _worldTransform; // Store the world transform
		MathLibrary::Matrix* _cameraTransform; // Store the camera transform					
		MathLibrary::Matrix* _perspectiveTransform; // Store the perspective transform
		MathLibrary::Matrix* _screenTransform; // Store the screen transform

		MathLibrary::Vector* _position; // Camera's position within the scene
		MathLibrary::Vector* _rotation; // Camera's rotation around the world origin

	private:
		float _nearDistance; // Near frustum
		float _farDistance; // Far frustum
		int _viewportWidth; // Width of viewport
		int _viewportHeight; // Height of viewport

		MathLibrary::Matrix* _temp1;
		MathLibrary::Matrix* _temp2;
		MathLibrary::Matrix* _temp3;

		float _fov; // FOV
	};
}

#endif // CAMERA_H