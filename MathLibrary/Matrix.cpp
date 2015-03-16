#include "Matrix.h"
#include "Vector.h"

#include <stdexcept>

// Index variables for loops - up to three 'for' loops running in synchrony
int index1 = 0; 
int index2 = 0; 
int index3 = 0;

namespace MathLibrary
{
	Matrix::Matrix()
	{
		// Default: Intialise with an Identity matrix
		InitialiseIdentity();
	}

	Matrix::Matrix (float _11, float _12, float _13, float _14, 
		float _21, float _22, float _23, float _24, 
		float _31, float _32, float _33, float _34, 
		float _41, float _42, float _43, float _44)
	{
		_matrix._data[0][0] = _11;
		_matrix._data[0][1] = _12;
		_matrix._data[0][2] = _13;
		_matrix._data[0][3] = _14;
		_matrix._data[1][0] = _21;
		_matrix._data[1][1] = _22;
		_matrix._data[1][2] = _23;
		_matrix._data[1][3] = _24;
		_matrix._data[2][0] = _31;
		_matrix._data[2][1] = _32;
		_matrix._data[2][2] = _33;
		_matrix._data[2][3] = _34;
		_matrix._data[3][0] = _41;
		_matrix._data[3][1] = _42;
		_matrix._data[3][2] = _43;
		_matrix._data[3][3] = _44;
	}

	Matrix::~Matrix()
	{

	}

	void Matrix::Initialise(float _11, float _12, float _13, float _14, 
		float _21, float _22, float _23, float _24, 
		float _31, float _32, float _33, float _34, 
		float _41, float _42, float _43, float _44)
	{
		_matrix._data[0][0] = _11;
		_matrix._data[0][1] = _12;
		_matrix._data[0][2] = _13;
		_matrix._data[0][3] = _14;
		_matrix._data[1][0] = _21;
		_matrix._data[1][1] = _22;
		_matrix._data[1][2] = _23;
		_matrix._data[1][3] = _24;
		_matrix._data[2][0] = _31;
		_matrix._data[2][1] = _32;
		_matrix._data[2][2] = _33;
		_matrix._data[2][3] = _34;
		_matrix._data[3][0] = _41;
		_matrix._data[3][1] = _42;
		_matrix._data[3][2] = _43;
		_matrix._data[3][3] = _44;
	}

	void Matrix::InitialiseNull()
	{
		// Clear all values
		for (index1 = 0; index1 < 4; index1++)
		{
			for (index2 = 0; index2 < 4; index2++)
			{
				_matrix._data[index1][index2] = 0;
			}
		}
	}

	void Matrix::InitialiseIdentity()
	{
		// Clear all values
		InitialiseNull();

		// Create an Identity matrix
		_matrix._data[0][0] = 1;
		_matrix._data[1][1] = 1;
		_matrix._data[2][2] = 1;
		_matrix._data[3][3] = 1;
	}

	bool Matrix::Equals(const Matrix&) const
	{
		// TODO: this
		return true;
	}

	void Matrix::Add(const Matrix& input1, const Matrix& input2, Matrix& result)
	{
		for (index1 = 0; index1 < 4; index1++)
		{
			for (index2 = 0; index2 < 4; index2++)
			{
				result._matrix._data[index1][index2] = input1._matrix._data[index1][index2] + input2._matrix._data[index1][index2];
			}
		}
	}

	void Matrix::Subtract(const Matrix& input1, const Matrix& input2, Matrix& result)
	{
		for (index1 = 0; index1 < 4; index1++)
		{
			for (index2 = 0; index2 < 4; index2++)
			{
				result._matrix._data[index1][index2] = input1._matrix._data[index1][index2] - input2._matrix._data[index1][index2];
			}
		}
	}

	void Matrix::Multiply(const Matrix& input1, float scalar, Matrix& result)
	{	
		for (index1 = 0; index1 < 4; index1++)
		{
			for (index2 = 0; index2 < 4; index2++)
			{
				result._matrix._data[index1][index2] = input1._matrix._data[index1][index2] * scalar;
			}
		}
	}

	void Matrix::Multiply(const Matrix& input1, const Matrix& input2, Matrix& result)
	{
		// Simplify?
		result._matrix._data[0][0] = (input1._matrix._data[0][0] * input2._matrix._data[0][0]) + (input1._matrix._data[0][1] * input2._matrix._data[1][0]) + (input1._matrix._data[0][2] * input2._matrix._data[2][0]) + (input1._matrix._data[0][3] * input2._matrix._data[3][0]);
		result._matrix._data[0][1] = (input1._matrix._data[0][0] * input2._matrix._data[0][1]) + (input1._matrix._data[0][1] * input2._matrix._data[1][1]) + (input1._matrix._data[0][2] * input2._matrix._data[2][1]) + (input1._matrix._data[0][3] * input2._matrix._data[3][1]);
		result._matrix._data[0][2] = (input1._matrix._data[0][0] * input2._matrix._data[0][2]) + (input1._matrix._data[0][1] * input2._matrix._data[1][2]) + (input1._matrix._data[0][2] * input2._matrix._data[2][2]) + (input1._matrix._data[0][3] * input2._matrix._data[3][2]);
		result._matrix._data[0][3] = (input1._matrix._data[0][0] * input2._matrix._data[0][3]) + (input1._matrix._data[0][1] * input2._matrix._data[1][3]) + (input1._matrix._data[0][2] * input2._matrix._data[2][3]) + (input1._matrix._data[0][3] * input2._matrix._data[3][3]);
		result._matrix._data[1][0] = (input1._matrix._data[1][0] * input2._matrix._data[0][0]) + (input1._matrix._data[1][1] * input2._matrix._data[1][0]) + (input1._matrix._data[1][2] * input2._matrix._data[2][0]) + (input1._matrix._data[1][3] * input2._matrix._data[3][0]);
		result._matrix._data[1][1] = (input1._matrix._data[1][0] * input2._matrix._data[0][1]) + (input1._matrix._data[1][1] * input2._matrix._data[1][1]) + (input1._matrix._data[1][2] * input2._matrix._data[2][1]) + (input1._matrix._data[1][3] * input2._matrix._data[3][1]);
		result._matrix._data[1][2] = (input1._matrix._data[1][0] * input2._matrix._data[0][2]) + (input1._matrix._data[1][1] * input2._matrix._data[1][2]) + (input1._matrix._data[1][2] * input2._matrix._data[2][2]) + (input1._matrix._data[1][3] * input2._matrix._data[3][2]);
		result._matrix._data[1][3] = (input1._matrix._data[1][0] * input2._matrix._data[0][3]) + (input1._matrix._data[1][1] * input2._matrix._data[1][3]) + (input1._matrix._data[1][2] * input2._matrix._data[2][3]) + (input1._matrix._data[1][3] * input2._matrix._data[3][3]);
		result._matrix._data[2][0] = (input1._matrix._data[2][0] * input2._matrix._data[0][0]) + (input1._matrix._data[2][1] * input2._matrix._data[1][0]) + (input1._matrix._data[2][2] * input2._matrix._data[2][0]) + (input1._matrix._data[2][3] * input2._matrix._data[3][0]);
		result._matrix._data[2][1] = (input1._matrix._data[2][0] * input2._matrix._data[0][1]) + (input1._matrix._data[2][1] * input2._matrix._data[1][1]) + (input1._matrix._data[2][2] * input2._matrix._data[2][1]) + (input1._matrix._data[2][3] * input2._matrix._data[3][1]);
		result._matrix._data[2][2] = (input1._matrix._data[2][0] * input2._matrix._data[0][2]) + (input1._matrix._data[2][1] * input2._matrix._data[1][2]) + (input1._matrix._data[2][2] * input2._matrix._data[2][2]) + (input1._matrix._data[2][3] * input2._matrix._data[3][2]);
		result._matrix._data[2][3] = (input1._matrix._data[2][0] * input2._matrix._data[0][3]) + (input1._matrix._data[2][1] * input2._matrix._data[1][3]) + (input1._matrix._data[2][2] * input2._matrix._data[2][3]) + (input1._matrix._data[2][3] * input2._matrix._data[3][3]);
		result._matrix._data[3][0] = (input1._matrix._data[3][0] * input2._matrix._data[0][0]) + (input1._matrix._data[3][1] * input2._matrix._data[1][0]) + (input1._matrix._data[3][2] * input2._matrix._data[2][0]) + (input1._matrix._data[3][3] * input2._matrix._data[3][0]);
		result._matrix._data[3][1] = (input1._matrix._data[3][0] * input2._matrix._data[0][1]) + (input1._matrix._data[3][1] * input2._matrix._data[1][1]) + (input1._matrix._data[3][2] * input2._matrix._data[2][1]) + (input1._matrix._data[3][3] * input2._matrix._data[3][1]);
		result._matrix._data[3][2] = (input1._matrix._data[3][0] * input2._matrix._data[0][2]) + (input1._matrix._data[3][1] * input2._matrix._data[1][2]) + (input1._matrix._data[3][2] * input2._matrix._data[2][2]) + (input1._matrix._data[3][3] * input2._matrix._data[3][2]);
		result._matrix._data[3][3] = (input1._matrix._data[3][0] * input2._matrix._data[0][3]) + (input1._matrix._data[3][1] * input2._matrix._data[1][3]) + (input1._matrix._data[3][2] * input2._matrix._data[2][3]) + (input1._matrix._data[3][3] * input2._matrix._data[3][3]);
	}

	void Matrix::Transform(const Matrix& transform, const Vector& verts, Vector& transformed)
	{
		//Matrix multiply with a vector
		//Simplify?

		transformed._vector._x =	(transform._matrix._data[0][0] * verts._vector._x) + 
									(transform._matrix._data[0][1] * verts._vector._y) + 
									(transform._matrix._data[0][2] * verts._vector._z) + 
									(transform._matrix._data[0][3] * verts._vector._w);

		transformed._vector._y =	(transform._matrix._data[1][0] * verts._vector._x) + 
									(transform._matrix._data[1][1] * verts._vector._y) + 
									(transform._matrix._data[1][2] * verts._vector._z) + 
									(transform._matrix._data[1][3] * verts._vector._w);

		transformed._vector._z =	(transform._matrix._data[2][0] * verts._vector._x) + 
									(transform._matrix._data[2][1] * verts._vector._y) + 
									(transform._matrix._data[2][2] * verts._vector._z) + 
									(transform._matrix._data[2][3] * verts._vector._w);

		transformed._vector._w =	(transform._matrix._data[3][0] * verts._vector._x) + 
									(transform._matrix._data[3][1] * verts._vector._y) + 
									(transform._matrix._data[3][2] * verts._vector._z) + 
									(transform._matrix._data[3][3] * verts._vector._w);
	}

	std::ostream& MathLibrary::operator << (std::ostream& out, const Matrix& mat)
	{
		out << "<" << mat._matrix._data[0][0] << "," << mat._matrix._data[0][1] << "," << mat._matrix._data[0][2] << "," << mat._matrix._data[0][3] << ">" << std::endl;
		out << "<" << mat._matrix._data[1][0] << "," << mat._matrix._data[1][1] << "," << mat._matrix._data[1][2] << "," << mat._matrix._data[1][3] << ">" << std::endl;
		out << "<" << mat._matrix._data[2][0] << "," << mat._matrix._data[2][1] << "," << mat._matrix._data[2][2] << "," << mat._matrix._data[2][3] << ">" << std::endl;
		out << "<" << mat._matrix._data[3][0] << "," << mat._matrix._data[3][1] << "," << mat._matrix._data[3][2] << "," << mat._matrix._data[3][3] << ">" << std::endl;
		
		return out;
	}
}
