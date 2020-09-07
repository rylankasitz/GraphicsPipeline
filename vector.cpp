#include "stdafx.h"
#include "vector.h"

#define _USE_MATH_DEFINES 
#include "math.h"

#include <iostream>
#include "matrix.h"

Vector Vector::XAXIS = Vector(1, 0, 0);
Vector Vector::YAXIS = Vector(0, 1, 0);
Vector Vector::ZAXIS = Vector(0, 0, 1);
Vector Vector::ZERO = Vector(0, 0, 0);

Vector::Vector(float _x, float _y, float _z) {

	x = _x;
	y = _y;
	z = _z;

}

#pragma region Operators

const float& Vector::operator[](const int i) const {

	switch (i) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		cerr << "Error no coord for index " << i << endl;
	}

}

float& Vector::operator[](const int i) {

	switch (i) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	default:
		cerr << "Error no coord for index " << i << endl;
	}

}

Vector Vector::operator+(Vector v) {

	return Vector(x + v.x, y + v.y, z + v.z);

}

Vector Vector::operator-(Vector v) {

	return Vector(x - v.x, y - v.y, z - v.z);

}

float Vector::operator*(Vector v) {

	return x * v.x + y * v.y + z * v.z;

}

Vector Vector::operator^(Vector v) {

	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);

}

Vector Vector::operator*(float s) {

	return Vector(x * s, y * s, z * s);

}

Vector Vector::operator/(float s) {

	return Vector(x / s, y / s, z / s);

}

ostream& operator<<(ostream& strm, const Vector v) {

	return strm << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";

}

istream& operator>>(istream& input, Vector& v) {

	return input >> v[0] >> v[1] >> v[2];

}

#pragma endregion Operator overide methods

#pragma region Public Methods

void Vector::Normalize() {

	float l = Length();

	x = x / l;
	y = y / l;
	z = z / l;
}

Vector Vector::Normalized() {

	return (*this) / Length();

}


float Vector::Length() {

	Vector v = (*this);
	return sqrt(v * v);

}

void Vector::RotatePoint(Vector origin, Vector direction, float angle) {

	Vector aAxis = direction.Normalized();

	float dotX = abs(XAXIS * aAxis);
	float dotY = abs(YAXIS * aAxis);

	Vector bAxis, cAxis;
	if (dotX < dotY) {
		bAxis = (XAXIS ^ aAxis).Normalized();
		cAxis = (aAxis ^ bAxis).Normalized();
	}
	else {
		bAxis = (YAXIS ^ aAxis).Normalized();
		cAxis = (aAxis ^ bAxis).Normalized();
	}

	Vector transPoint = Vector(x, y, z) - origin;
	Matrix transM = Matrix(aAxis, bAxis, cAxis);
	Matrix rotation = Matrix();

	rotation.SetRotationMatrixX(angle * (M_PI / 180));

	transPoint = transM * transPoint;
	transPoint = rotation * transPoint;

	transM.Transpose();
	transPoint = transM * transPoint;

	x = transPoint[0] + origin[0];
	y = transPoint[1] + origin[1];
	z = transPoint[2] + origin[2];
}

void Vector::RotateVector(Vector axis, float angle) {

	Vector v = (*this);

	v.RotatePoint(ZERO, axis, angle);

	x = v[0];
	y = v[1];
	z = v[2];
}

#pragma endregion Preforms vector operations

#pragma region Private Methods

#pragma endregion
