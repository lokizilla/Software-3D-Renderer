#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

namespace MathLibrary
{
	class Matrix
	{
	public: 
		Matrix(void); // Default constructor
		Matrix(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44); // Directly initialise matrix with sixteen values
		~Matrix(void); // Destructor

		void Initialise(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44); // Fill the matrix with the supplied values
		void InitialiseNull(); // Create a Null matrix
		void InitialiseIdentity(); // Create the Identity matrix

		// Equality
		bool Equals(const Matrix&) const; // Operator used to check whether two Matrices are equal

		// Arithmetic
		static void Add(const Matrix& input1, const Matrix& input2, Matrix& result); // Add the supplied Matrix
		static void Subtract(const Matrix& input1, const Matrix& input2, Matrix& result); // Subtract the supplied Matrix
		static void Multiply(const Matrix& input1, float scalar, Matrix& result); // Multiply by scalar
		static void Multiply(const Matrix& input1, const Matrix& input2, Matrix& result); // Multiply by another matrix
		static void Transform(const Matrix& input1, const Vector& input2, Vector& result); // Apply a vector transform

		// Debug
		friend std::ostream& operator<<(std::ostream&, const Matrix&); // Output operator

		// Data
		union matrix_t
		{
			float _data[4][4]; // 2D array to hold data
			struct
			{
				float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;
			};
		} _matrix;
	};
}

#endif // MATRIX_H
