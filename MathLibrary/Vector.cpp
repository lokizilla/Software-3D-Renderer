// Implementation for a 4-dimensional math vector class.

#include "Vector.h"

#include <math.h>		// Required for math functions
#include <stdexcept>

namespace MathLibrary
{
	Vector::Vector(void)
	{
		_vector._x = 0;
		_vector._y = 0;
		_vector._z = 0;
		_vector._w = 0;
	}

	Vector::Vector(float x, float y, float z, float w)
	{
		_vector._x = x;
		_vector._y = y;
		_vector._z = z;
		_vector._w = w;
	}	
	
	Vector::Vector(float x, float y, float z)
	{
		_vector._x = x;
		_vector._y = y;
		_vector._z = z;
	}

	Vector::Vector(const Vector& input)
	{
		_vector._x = input._vector._x;
		_vector._y = input._vector._y;
		_vector._z = input._vector._z;
		_vector._w = input._vector._w;
	}

	Vector::~Vector()
	{

	}

	Vector& Vector::operator = (Vector& input)
	{	
		_vector._x = input._vector._x;
		_vector._y = input._vector._y;
		_vector._z = input._vector._z;
		_vector._w = input._vector._w;

		return *this;
	}

	// ARITHMETIC OPERATIONS //------------------------------
	Vector Vector::Add(const Vector input)
	{		
		return Vector((_vector._x + input._vector._x),
			(_vector._y + input._vector._y),
			(_vector._z + input._vector._z),
			(_vector._w + input._vector._w));
	}

	Vector Vector::Subtract(const Vector input)
	{
		return Vector(_vector._x - input._vector._x,
			_vector._y - input._vector._y,
			_vector._z - input._vector._z,
			_vector._w - input._vector._w);
	}

	Vector Vector::Multiply(float scalar)
	{
		return Vector(_vector._x * scalar, 
			_vector._y * scalar, 
			_vector._z * scalar, 
			_vector._w * scalar);
	}

	Vector Vector::Divide(const Vector& input1, const Vector& input2)
	{
		return Vector(input1._vector._x / input2._vector._x,
			input1._vector._y / input2._vector._y,
			input1._vector._z / input2._vector._z,
			input1._vector._w / input2._vector._w);

		//return Vector(_vector._x / scalar, _vector._y / scalar, _vector._z / scalar);
	}

	// OTHER OPERATIONS //-----------------------------------
	float Vector::Magnitude()
	{
		// magnitude:
		// x^2 y^2 z^2 w^2
		_vector._x = _vector._x * _vector._x;
		_vector._y = _vector._y * _vector._y;
		_vector._z = _vector._z * _vector._z;
		_vector._w = _vector._w * _vector._w;

		// sqrt
		return sqrt(_vector._x + _vector._y + _vector._z + _vector._w);

		//return 0.0f;
	}

	Vector Vector::MakeUnit()
	{
		//unit: vector / magnitude
		float _mag = 0;
		_mag = Magnitude();

		_vector._x = _vector._x / _mag;
		_vector._y = _vector._y / _mag;
		_vector._z = _vector._z / _mag;
		_vector._w = _vector._w / _mag;

		return *this;

		// Magnitude of 1
		//_vector._x = 1;
		//_vector._y = 1;
		//_vector._z = 1;
		//_vector._w = 1;
	}

	float Vector::DotProduct(Vector input)
	{		
		return float((input._vector._x * _vector._x) + 
			(input._vector._y * _vector._y) + 
			(input._vector._z * _vector._z));
	}

	Vector Vector::CrossProduct(Vector input)
	{
		return Vector((_vector._y * input._vector._z) - (_vector._z * input._vector._y),
			(_vector._z * input._vector._x) - (_vector._x * input._vector._z), 
			(_vector._x * input._vector._y) - (_vector._y * input._vector._x), 
			0);
	}

	float Vector::Length()
	{
		return sqrt((_vector._x * _vector._x) + (_vector._y * _vector._y) + (_vector._z * _vector._z));
	}

	Vector Vector::Normalise()
	{
		float length = Length();
		
		if (length > 0)
		{
			_vector._x /= length;
			_vector._y /= length;
			_vector._z /= length;
			//_vector._w = length;
		}

		return *this;
	}

	std::ostream& MathLibrary::operator << (std::ostream& out, const Vector& vec)
	{
		out << "<" << vec._vector._x << ", " << vec._vector._y << ", " << vec._vector._z << ", " << vec._vector._w << ">" << std::endl;

		return out;
	}
}
