#include "stdafx.h"
#include "Object.h"
#include <algorithm> // Contains definitions for the sort

namespace RenderLibrary
{
	Object::Object()
	{
		// Initialise member variables
		_numberOfVerts = 0;
		_numberOfPolys = 0;
		_dotProduct = 0;

		_position = new MathLibrary::Vector();
		_rotation = new MathLibrary::Vector();
	}

	Object::~Object()
	{
		// Delete member variables
		delete [] _originalVerts;
		delete [] _worldSpaceVerts;
		delete [] _cameraSpaceVerts;
		delete [] _projectionSpaceVerts;
		delete [] _screenSpaceVerts;

		delete _position;
		delete _rotation;
	}

	//Object::Object(const Object& obj)
	//{
	//	
	//}

	void Object::Initialise()
	{
		// Create the arrays with the sizes supplied by the MD2 Loader
		_polys.reserve(_numberOfPolys);
		_originalVerts = new MathLibrary::Vector[_numberOfVerts];
		_worldSpaceVerts = new MathLibrary::Vector[_numberOfVerts];
		_cameraSpaceVerts = new MathLibrary::Vector[_numberOfVerts];
		_projectionSpaceVerts = new MathLibrary::Vector[_numberOfVerts];
		_screenSpaceVerts = new MathLibrary::Vector[_numberOfVerts];

		// Set standard reflectivity to full
		_reflectivity._red = 1.0f;
		_reflectivity._green = 1.0f;
		_reflectivity._blue = 1.0f;
	}

	void Object::Transform(const MathLibrary::Matrix& transform, E_TRANSFORM_VERTS eTransformArgs)
	{
		_index = 0;

		switch (eTransformArgs)
		{
		case 0: // WORLD_SPACE_VERTS
			for (_index = 0; _index < _numberOfVerts; ++_index) // For all verts:
			{
				// Copy out the original verts to the transform set
				_worldSpaceVerts[_index]._vector._x = _originalVerts[_index]._vector._x;
				_worldSpaceVerts[_index]._vector._y = _originalVerts[_index]._vector._y;
				_worldSpaceVerts[_index]._vector._z = _originalVerts[_index]._vector._z;
				_worldSpaceVerts[_index]._vector._w = _originalVerts[_index]._vector._w;

				transform.Transform(transform, _originalVerts[_index], _worldSpaceVerts[_index]); // Apply the transform matrix
			}
			break;
		case 1: // CAMERA_SPACE_VERTS
			for (_index = 0; _index < _numberOfVerts; ++_index) // For all verts:
			{								
				transform.Transform(transform, _worldSpaceVerts[_index], _cameraSpaceVerts[_index]); // Apply the transform
			}
			break;

		case 2: // PROJECTION_SPACE_VERTS
			for (_index = 0; _index < _numberOfVerts; ++_index) // For all verts:
			{
				transform.Transform(transform, _cameraSpaceVerts[_index], _projectionSpaceVerts[_index]); // Apply the transform
			}
			break;
		case 3: // SCREEN_SPACE_VERTS
			for (_index = 0; _index < _numberOfVerts; ++_index) // For all verts:
			{
				transform.Transform(transform, _projectionSpaceVerts[_index], _screenSpaceVerts[_index]); // Apply the transform
			}
			break;
		}
	}

	void Object::DeHomogenise()
	{
		_index = 0;

		// Divide transformed verts by the w co-ordinate
		for (_index = 0; _index < _numberOfVerts; ++_index)
		{
			_projectionSpaceVerts[_index]._vector._x = _projectionSpaceVerts[_index]._vector._x / _projectionSpaceVerts[_index]._vector._w; // X
			_projectionSpaceVerts[_index]._vector._y = _projectionSpaceVerts[_index]._vector._y / _projectionSpaceVerts[_index]._vector._w; // Y
			_projectionSpaceVerts[_index]._vector._z = _projectionSpaceVerts[_index]._vector._z / _projectionSpaceVerts[_index]._vector._w; // Z
			_projectionSpaceVerts[_index]._vector._w = _projectionSpaceVerts[_index]._vector._w / _projectionSpaceVerts[_index]._vector._w; // W
		}
	}

	void Object::CalculateBackfaces(const Camera& cam)
	{
		static MathLibrary::Vector a;
		static MathLibrary::Vector b;

		static MathLibrary::Vector tempVert1;
		static MathLibrary::Vector tempVert2;
		static MathLibrary::Vector tempVert3;
		//static int temp = 0;
		static RenderLibrary::Polygon* poly;

		// for each polygon
		for (_index = 0; _index < _numberOfPolys; ++_index)
		{
			poly = _polys[_index];
			
			// get the 3 indices of the vertices that make up the polygon
			// lookup the verts from those indices on the object

			// construct vector a by subtracting vertex 1 from vertex 0.
			a = _worldSpaceVerts[poly->_data[0]];  // vert 0
			a = a.Subtract(_worldSpaceVerts[poly->_data[1]]); // vert 1

			// construct vector b by subtracting vertex 2 from the 0.
			b = _worldSpaceVerts[poly->_data[0]]; // vert 0
			b = b.Subtract(_worldSpaceVerts[poly->_data[2]]); // vert 2

			// calculate the normal from vector a and b (crossproduct)
			a = a.CrossProduct(b); // a is now the normal

			// Store the current normal for reference later in the pipeline
			poly->_currentNormal = a;

			// Create eye-vector to viewpoint (vertex 0 to the camera position)
			MathLibrary::Vector eyeVector = _worldSpaceVerts[poly->_data[0]]; // vert 0
			eyeVector = eyeVector.Subtract(*cam._position);
			
			a = a.Normalise();
			eyeVector = eyeVector.Normalise();

			// Take dot product of these two vectors
			_dotProduct = a.DotProduct(eyeVector);

			_polys[_index]->_backface = false; // Reset the tag

			// if result < 0
			if (_dotProduct <= 0.0f)
			{
				// tag the polygon as back facing
				poly->_backface = true;
			}
		}
	}

	bool SortPolys(Polygon* p1, Polygon* p2)
	{
		return p1->_depth < p2->_depth;
	}

	void Object::DepthSort()
	{
		_index = 0;

		static MathLibrary::Vector tempVert1;
		static MathLibrary::Vector tempVert2;
		static MathLibrary::Vector tempVert3;
		static int temp = 0;
		static RenderLibrary::Polygon* poly;
		
		for (_index = 0; _index < _numberOfPolys; ++_index) // Step through each Polygon in the Object
		{
			if (_polys[_index]->_backface == false) // If polygon isn't back facing...
			{
				poly = _polys[_index];
				
				// get each of the indices for each Polygon
				// look up the transformed verts in the Object
				temp = poly->_data[0];
				tempVert1._vector._x = _cameraSpaceVerts[temp]._vector._x;
				tempVert1._vector._y = _cameraSpaceVerts[temp]._vector._y;
				tempVert1._vector._z = _cameraSpaceVerts[temp]._vector._z;
				tempVert1._vector._w = _cameraSpaceVerts[temp]._vector._w;

				temp = poly->_data[1];
				tempVert2._vector._x = _cameraSpaceVerts[temp]._vector._x;
				tempVert2._vector._y = _cameraSpaceVerts[temp]._vector._y;
				tempVert2._vector._z = _cameraSpaceVerts[temp]._vector._z;
				tempVert2._vector._w = _cameraSpaceVerts[temp]._vector._w;

				temp = poly->_data[2];
				tempVert3._vector._x = _cameraSpaceVerts[temp]._vector._x;
				tempVert3._vector._y = _cameraSpaceVerts[temp]._vector._y;
				tempVert3._vector._z = _cameraSpaceVerts[temp]._vector._z;
				tempVert3._vector._w = _cameraSpaceVerts[temp]._vector._w;

				// calculate an average z depth for the Polygon verts, and store this value back in the Polygon
				poly->_depth = (tempVert1._vector._z + tempVert2._vector._z + tempVert3._vector._z) / 3;
			}
		}

		// sort() the Polygon array to put furthest away first
		std::sort(_polys.begin(), _polys.end(), SortPolys);

		// Calculate an average depth for the object
		for (_index = 0; _index < _numberOfPolys; ++_index)
		{
			poly = _polys[_index];
			
			_averageDepth += poly->_depth;
		}

		_averageDepth /= _numberOfPolys;
	}

	int Object::GetNumberOfPolys()
	{
		return _numberOfPolys;
	}

	int Object::GetNumberOfVerts()
	{
		return _numberOfVerts;
	}

	void Object::SetNumberOfPolys(int number)
	{
		_numberOfPolys = number;
	}

	void Object::SetNumberOfVerts(int number)
	{
		_numberOfVerts = number;
	}
}