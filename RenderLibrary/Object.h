// Object header file.

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include "Camera.h"
#include "Matrix.h"
#include "Vector.h"
#include "Polygon.h"
#include "LightColour.h"

enum E_TRANSFORM_VERTS{WORLD_SPACE_VERTS, CAMERA_SPACE_VERTS, PROJECTION_SPACE_VERTS, SCREEN_SPACE_VERTS}; 

namespace RenderLibrary
{
	class Object
	{
	public:
		Object(); // Default constructor
		~Object(); // Destructor
		//const Object(const Object& obj); // Copy constructor

		void Initialise(); // Creates arrays with the given sizes
		void Transform(const MathLibrary::Matrix& transform, enum E_TRANSFORM_VERTS eTransformArgs); // Transforms the object's verts using the supplied matrix
		void DeHomogenise(); // De-homogenise the transformed verts

		void CalculateBackfaces(const Camera& cam); // Call to calculate back face culling
		void DepthSort(); // Depth-sort the object

		float _dotProduct; // Somewhere to return a dot product to

		int _index; // Loop reference

		MathLibrary::Vector* _originalVerts; // Pointer to an array of vertex data
		MathLibrary::Vector* _worldSpaceVerts; // Pointer to an array of transformed vertex data (world space)
		MathLibrary::Vector* _cameraSpaceVerts; // Pointer to an array of transformed vertex data (camera space)
		MathLibrary::Vector* _projectionSpaceVerts; // Pointer to an array of transformed vertex data (projection space)
		MathLibrary::Vector* _screenSpaceVerts; // Pointer to an array of transformed vertex data (screen space)
		std::vector<RenderLibrary::Polygon*> _polys; // A set of polygon data

		int GetNumberOfPolys(); // Return the number of polys in the object
		int GetNumberOfVerts();	// Return the number of verts in the object
		void SetNumberOfPolys(int number); // Set the number of polys in the object
		void SetNumberOfVerts(int number); // Set the number of verts in the object
				
		MathLibrary::Vector* _position; // Position
		MathLibrary::Vector* _rotation; // Direction

		float _averageDepth; // Store an average depth value

		union reflectivity_t
		{
			float _reflectivity[3];
			struct
			{
				float _red, _green, _blue;
			};
		} _reflectivity;
		
		const char* _filename; // Store the filename of the model

	private:
		int _numberOfVerts; // Stores the number of verts being held in the array
		int _numberOfPolys; // Stores the number of polys being held in the array
	};
}

#endif // OBJECT_H