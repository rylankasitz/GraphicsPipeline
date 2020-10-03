#include "stdafx.h"
#include "matrix.h";

#define _USE_MATH_DEFINES 
#include "math.h"
#include "ppc.h"

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

Matrix Matrix::GetEdgeEQS() {
	Matrix ret;

	Vector pvs[3];
	pvs[0] = r0;
	pvs[1] = r1;
	pvs[2] = r2;

	for (int i = 0; i < 3; i++) {
		ret[i] = getEdgeEQ(pvs[i], pvs[(i + 1) % 3], pvs[(i + 2) % 3]);
	}

	return ret;
}

Matrix Matrix::GetSSIM() {

	Matrix ret = (*this);
	ret.SetColumn(2, Vector(1.0f, 1.0f, 1.0f));

	return ret.Inverted();
}

Matrix Matrix::GetPIM(PPC * ppc) {

	Matrix ret = (*this);

	ret[0] = ret[0] - ppc->C;
	ret[1] = ret[1] - ppc->C;
	ret[2] = ret[2] - ppc->C;

	return ret.Transposed().Inverted() * ppc->GetPerspectiveM();
}

#pragma endregion Preforms matrix operations

#pragma region Private Methods

Vector Matrix::getEdgeEQ(Vector v1, Vector v2, Vector v3) {

	float x0, x1, y0, y1;
	x0 = v1[0];
	x1 = v2[0];
	y0 = v1[1];
	y1 = v2[1];

	Vector ret;
	ret[0] = y1 - y0;
	ret[1] = x0 - x1;
	ret[2] = x0 * (y0 - y1) + y0 * (x1 - x0);

	Vector v2p(v3); v2p[2] = 1.0f;
	if ((ret * v2p) < 0.0f)
		ret = ret * -1.0f;

	return ret;
}

#pragma endregion