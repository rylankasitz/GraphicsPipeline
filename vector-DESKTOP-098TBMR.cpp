#include "stdafx.h"

#include "vector.h"
#include "math.h"
#include "matrix.h";

#include <iostream>

using namespace std;

Vector Vector::XAXIS = Vector(1, 0, 0);
Vector Vector::YAXIS = Vector(0, 1, 0);
Vector Vector::ZAXIS = Vector(0, 0, 1);

Vector::Vector() {

	x = 0;
	y = 0;
	z = 0;

}

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

std::ostream& operator<<(std::ostream& strm, const Vector v) {

	return strm << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";

}

#pragma endregion Operator overide methods

#pragma region Public Methods

void Vector::Normalize() {

	float l = Length();

	x = x / l;
	y = y / l;
	z = z / l;
}


float Vector::Length() {

	return sqrt(x * x + y * y + z * z);

}

void Vector::RotatePoint(Vector origin, Vector direction, float angle) {

	float dotX = abs(XAXIS * direction);
	float dotZ = abs(ZAXIS * direction);

	Vector bAxis;
	if (dotX < dotZ) {
		bAxis = XAXIS ^ direction;
		bAxis.Normalize();
	}
	else {
		bAxis = ZAXIS ^ direction;
	}

	bAxis.Normalize();
	
	Vector cAxis = direction ^ bAxis;
	cAxis.Normalize();

	Vector transPoint = Vector(x, y, z) - origin;

	Matrix rotation = Matrix();
	rotation.SetRotationMatrix(angle);

	transPoint = rotation * transPoint;

	x = transPoint[0] + origin[0];
	y = transPoint[1] + origin[1];
	z = transPoint[2] + origin[2];
}

void Vector::RotateVector(Vector axis) {

}

#pragma endregion Preforms vector operations

#pragma region Private Methods

#pragma endregion
