#include "stdafx.h"
#include "matrix.h";

#define _USE_MATH_DEFINES 
#include "math.h"

#include <iostream>

using namespace std;

Matrix::Matrix() {
	r0 = Vector();
	r1 = Vector();
	r2 = Vector();
}

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
	Vector mr0 = Vector(getCofactor(0, 0), -getCofactor(0, 1), getCofactor(0, 2));
	Vector mr1 = Vector(-getCofactor(1, 0), getCofactor(1, 1), -getCofactor(1, 2));
	Vector mr2 = Vector(getCofactor(2, 0), -getCofactor(2, 1), getCofactor(2, 2));

	Matrix mm = Matrix(mr0, mr1, mr2);

	mm.Transpose();

	float det = r0[0] * mr0[0] - r0[1] * -mr0[1] + r0[2] * mr0[2];

	r0 = mm[0] / det;
	r1 = mm[1] / det;
	r2 = mm[2] / det;
}

void Matrix::Transpose() {
	Vector nr0 = r0;
	Vector nr1 = r1;
	Vector nr2 = r2;

	SetColumn(0, nr0);
	SetColumn(1, nr1);
	SetColumn(2, nr2);
}

void Matrix::SetRotationMatrix(float degrees) {

	float radians = degrees * (M_PI / 180);

	r0 = Vector(cos(radians), 0, sin(radians));
	r1 = Vector(0, 1, 0);
	r2 = Vector(-sin(radians), 0, cos(radians));
}

#pragma endregion Preforms matrix operations

#pragma region Private Methods

float Matrix::getCofactor(int row, int col) {

	float det[4];
	int i = 0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (r != row && c != col) {
				det[i] = GetColumn(c)[r];
				i++;

				if (i == 4) break;
			}
		}
	}

	return det[0] * det[3] - det[1] * det[2];
}

#pragma endregion