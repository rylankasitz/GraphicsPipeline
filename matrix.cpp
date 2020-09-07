#include "stdafx.h"
#include "matrix.h";

#define _USE_MATH_DEFINES 
#include "math.h"

#include <iostream>

using namespace std;

Matrix::Matrix(Vector _r0, Vector _r1, Vector _r2) {

	r0 = _r0;
	r1 = _r1;
	r2 = _r2;

	rows = 3;
	cols = 3;
}

#pragma region Operators

const Vector& Matrix::operator[](const int i) const {

	switch (i) {
	case 0:
		return r0;
		break;
	case 1:
		return r1;
		break;
	case 2:
		return r2;
		break;
	default:
		cerr << "Error no row for index " << i << endl;
	}

}

Vector& Matrix::operator[](const int i) {

	switch (i) {
	case 0:
		return r0;
		break;
	case 1:
		return r1;
		break;
	case 2:
		return r2;
		break;
	default:
		cerr << "Error no row for index " << i << endl;
	}

}

Vector Matrix::operator*(Vector v) {

	return Vector(r0 * v, r1 * v, r2 * v);

}

Matrix Matrix::operator*(Matrix v) {
	return Matrix(Vector(r0 * v.GetColumn(0), r0 * v.GetColumn(1), r0 * v.GetColumn(2)),
		Vector(r1 * v.GetColumn(0), r1 * v.GetColumn(1), r1 * v.GetColumn(2)),
		Vector(r2 * v.GetColumn(0), r2 * v.GetColumn(1), r2 * v.GetColumn(2)));
}

std::ostream& operator<<(std::ostream& strm, const Matrix m) {

	return strm << m[0] << "\n" << m[1] << "\n" << m[2];

}

istream& operator>>(istream& input, Matrix& m) {

	cerr << "\nEnter row 1: ";
	input >> m[0];
	cerr << "Enter row 2: ";
	input >> m[1];
	cerr << "Enter row 3: ";
	input >> m[2];

	return input;

}

#pragma endregion Operator overide methods

#pragma region Public Methods

Vector Matrix::GetColumn(int i) {

	return Vector(r0[i], r1[i], r2[i]);

}

void Matrix::SetColumn(int i, Vector v) {

	r0[i] = v[0];
	r1[i] = v[1];
	r2[i] = v[2];

}

void Matrix::Invert() {

	Vector a = GetColumn(0), b = GetColumn(1), c = GetColumn(2);
	Vector _a = b ^ c; _a = _a / (a * _a);
	Vector _b = c ^ a; _b = _b / (b * _b);
	Vector _c = a ^ b; _c = _c / (c * _c);

	r0 = _a;
	r1 = _b;
	r2 = _c;
}

Matrix Matrix::Inverted() {

	Matrix ret = (*this);
	ret.Invert();

	return ret;
}

void Matrix::Transpose() {
	Vector nr0 = r0;
	Vector nr1 = r1;
	Vector nr2 = r2;

	SetColumn(0, nr0);
	SetColumn(1, nr1);
	SetColumn(2, nr2);
}

Matrix Matrix::Transposed() {
	Matrix ret = (*this);
	
	ret.Transpose();

	return ret;
}

void Matrix::SetRotationMatrixX(float radians) {

	r0 = Vector(1, 0, 0);
	r1 = Vector(0, cos(radians), -sin(radians));
	r2 = Vector(0, sin(radians), cos(radians));
}

void Matrix::SetRotationMatrixY(float radians) {

	r0 = Vector(cos(radians), 0, sin(radians));
	r1 = Vector(0, 1, 0);
	r2 = Vector(-sin(radians), 0, cos(radians));
}

void Matrix::SetRotationMatrixZ(float radians) {

	r0 = Vector(cos(radians), -sin(radians), 0);
	r1 = Vector(sin(radians), cos(radians), 0);
	r2 = Vector(0, 0, 1);
}

#pragma endregion Preforms matrix operations

#pragma region Private Methods

#pragma endregion