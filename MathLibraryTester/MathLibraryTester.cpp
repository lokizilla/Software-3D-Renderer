// Math Library Tester.
// Designed to provide hard-coded tests for the Intro3D2009 Math Library.
// Requirements: MathLibrary.

#include "Vector.h"
#include "Matrix.h"

using namespace MathLibrary;
using namespace std;

int main(/*int argc, _TCHAR* argv[]*/)
{

#pragma region Vector Testing

	Vector _vTest1;
	Vector _vTest2;
	Vector _vOutput;

	cout << "------- Vector Testing -------" << endl;

	#pragma region Add
	_vTest1 = Vector(2,2,2,2);
	_vTest2 = Vector(3,4,5,6);
	_vOutput = _vTest1.Add(_vTest2);

	cout << "-- Addition" << endl;
	cout << "Vector 1: " << _vTest1 << "Vector 2: " << _vTest2 << endl;
	cout << "Added together: " << _vOutput << endl;
	#pragma endregion

	#pragma region Subtract
	_vTest1 = Vector(2,2,2,2);
	_vTest2 = Vector(3,4,5,6);
	_vOutput = _vTest1.Subtract(_vTest2);

	cout << "-- Subtraction" << endl;
	cout << "Vector 1: " << _vTest1 << "Vector 2: " << _vTest2 << endl;
	cout << "Vector 2 from Vector 1: " << _vOutput << endl;
	#pragma endregion

	#pragma region Multiply scalar
	_vTest1 = Vector(3,4,5,6);
	_vOutput = _vTest1.Multiply(3);

	cout << "-- Multiply by Scalar" << endl;
	cout << "Vector 1: " << _vTest1;
	cout << "Vector 1 * 3: " << _vOutput << endl;
	#pragma endregion

	#pragma region Divide
	_vTest1 = Vector(2,2,2,2);
	_vTest2 = Vector(3,4,5,6);
	_vOutput = _vTest1.Divide(_vTest1, _vTest2);

	cout << "-- Divide" << endl;
	cout << "Vector 1: " << _vTest1 << "Vector 2: " << _vTest2 << endl;
	cout << "Vector 1 / Vector 2: " << _vOutput << endl;
	#pragma endregion

	#pragma region Magnitude
	cout << "-- Magnitude" << endl;

	_vTest1 = Vector(3,4,5,6);
	cout << "Vector 1: " << _vTest1;
		
	float _flt;
	_flt = _vTest1.Magnitude();

	cout << "Magnitude: " << _flt << endl;
	#pragma endregion

	#pragma region Unit
	cout << "\n-- Unit Vector" << endl;

	_vTest1 = Vector(3,4,5,6);
	cout << "Vector 1: " << _vTest1;
	
	_vOutput = _vTest1.MakeUnit();

	cout << "Unit: " << _vOutput << endl;
	#pragma endregion	

	#pragma region Dot Product
	cout << "-- Dot Product" << endl;

	_vTest1 = Vector(2,2,2,2);
	_vTest2 = Vector(3,4,5,6);
	cout << "Vector 1: " << _vTest1 << "Vector 2: " << _vTest2 << endl;
	
	_flt = _vTest1.DotProduct(_vTest2);

	cout << "Dot Product: " << _flt << endl;
	#pragma endregion

	#pragma region Cross Product
	_vTest1 = Vector(2,2,2,2);
	_vTest2 = Vector(3,4,5,6);
	_vOutput = _vTest1.CrossProduct(_vTest2);

	cout << "\n-- Cross Product" << endl;
	cout << "Vector 1: " << _vTest1 << "Vector 2: " << _vTest2 << endl;
	cout << "Cross Product: " << _vOutput << endl;
	#pragma endregion

	#pragma region Normalise
	cout << "-- Normalise" << endl;

	_vTest1 = Vector(3,4,5,6);
	cout << "Vector 1: " << _vTest1;
	
	_vOutput = _vTest1.Normalise();

	cout << "Normalised: " << _vOutput << endl;
	#pragma endregion

#pragma endregion

#pragma region Matrix Testing

	cout << "\n------ Matrix Testing -------\n" << endl;
	
	Matrix _mTest1;
	Matrix _mTest2;
	Matrix _mOutput;

	_mTest1.InitialiseIdentity();
	_mTest2.InitialiseIdentity();
	_mOutput.InitialiseIdentity();

	cout << "-- Initialise to Identity Matrix" << endl;

	cout << _mTest1 << endl;
	
	#pragma region Equality
	#pragma endregion

	#pragma region Add
	_mTest1 = Matrix(2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3);
	_mTest2 = Matrix(3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2);

	cout << "-- Addition" << endl;
	cout << "Matrix 1: \n" << _mTest1 << endl;
	cout << "Matrix 2: \n" << _mTest2 << endl;

	_mOutput.Add(_mTest1, _mTest2, _mOutput);

	cout << "Added together: \n" << _mOutput << endl;
	#pragma endregion

	#pragma region Subtract
	_mTest2 = Matrix(2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3);
	_mTest1 = Matrix(5,8,6,4,7,5,1,8,3,2,5,9,6,8,4,7);

	cout << "-- Subtraction" << endl;
	cout << "Matrix 1: \n" << _mTest1 << endl;
	cout << "Matrix 2: \n" << _mTest2 << endl;

	_mOutput.Subtract(_mTest1, _mTest2, _mOutput);

	cout << "Matrix 1 - Matrix 2: \n" << _mOutput << endl;
	#pragma endregion

	#pragma region Multiply scalar
	_mTest1 = Matrix(2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3);

	cout << "-- Multiply by Scalar" << endl;
	cout << "Matrix 1: \n" << _mTest1 << endl;

	_mOutput.Multiply(_mTest1, 3, _mOutput);

	cout << "Matrix 1 * 3: \n" << _mOutput << endl;
	#pragma endregion

	#pragma region Multiply matrix
	_mTest1 = Matrix(2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3);
	_mTest2 = Matrix(3,4,5,6,7,8,9,10,9,8,7,6,5,4,3,2);

	cout << "-- Multiply by Second Matrix" << endl;
	cout << "Matrix 1: \n" << _mTest1 << endl;
	cout << "Matrix 2: \n" << _mTest2 << endl;

	_mOutput.Multiply(_mTest1, _mTest2, _mOutput);

	cout << "Matrix 1 * Matrix 2: \n" << _mOutput << endl;
	#pragma endregion

	#pragma region Transform
	_mTest1 = Matrix(2,3,4,5,6,7,8,9,10,9,8,7,6,5,4,3);
	_vTest1 = Vector(2,3,4,5);

	cout << "-- Transform by Vector" << endl;
	cout << "Matrix 1: \n" << _mTest1 << endl;
	cout << "Vector 1: " << _vTest1 << endl;

	_mOutput.Transform(_mTest1, _vTest2, _vOutput);

	cout << "Transform: " << _vOutput << endl;
	#pragma endregion

#pragma endregion

	// Stop the program auto-exiting
	cout << "Done. Close the window." << endl;

	int stop = 1;
	cin >> stop;

	return 0;
}
