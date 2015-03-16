// Header for a 4-dimensional math vector class.

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>  // std::ostream to implement operator<<()

namespace MathLibrary
{
	class Vector
	{
	public:
		Vector(void); // Default constructor
		Vector(float x, float y, float z, float w); // Parameterised constructor
		Vector(float x, float y, float z); // Parameterised constructor (3D)
		Vector(const Vector& input); // Copy constructor
		~Vector(void); // Destructor

		// Operators
		Vector& operator = (Vector& input); // Assignment operator

		// Arithmetic Operations
		Vector Add(const Vector input); // Add the vector to this object
		Vector Subtract(const Vector input); // Subtract the vector from this object
		Vector Multiply(float scalar); // Multiply this object by the scalar
		Vector Divide(const Vector& input1, const Vector& input2);	// Divide this object by the scalar

		// Other Operations
		float Magnitude(); // Compute the Magnitude of this object, returning a float
		Vector MakeUnit(); // Make this object into a unit vector
		float DotProduct(Vector input); // Compute the DotProduct with the input vector, returning a float
		Vector CrossProduct(Vector input); // Compute the CrossProduct with the input vector, returning a vector
		float Length();
		Vector Normalise(); // Normalise the vector

		friend std::ostream& operator << (std::ostream&, const Vector&); // Output Operator Implemented for Printing

		union _vector_t
		{
			float _data[4]; // Stores the values being held in the object (0 = x, 1 = y, 2 = z, 3 = w)
			struct  
			{
				float _x, _y, _z, _w;
			};
		} _vector;
	};
}

#endif // VECTOR_H
