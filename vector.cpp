#include "stdafx.h"
#include "vector.h"

#define _USE_MATH_DEFINES 
#include "math.h"

#include <iostream>
#include <algorithm>  
#include "matrix.h"

Vector Vector::XAXIS = Vector(1, 0, 0);
Vector Vector::YAXIS = Vector(0, 1, 0);
Vector Vector::ZAXIS = Vector(0, 0, 1);
Vector Vector::ZERO = Vector(0, 0, 0);
Vector Vector::ONES = Vector(1, 1, 1);

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

Vector Vector::operator/(Vector v) {

	return Vector(x / v[0], y / v[1], z / v[2]);

}

ostream& operator<<(ostream& strm, const Vector v) {

	return strm << v[0] << " " << v[1] << " " << v[2];

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

float clamp(float value, float minV, float maxV) {

	return max(minV, min(maxV, value));
}

void Vector::Clamp(Vector mins, Vector maxs) {

	x = clamp(x, mins[0], maxs[0]);
	y = clamp(y, mins[1], maxs[1]);
	z = clamp(z, mins[2], maxs[2]);
}

void Vector::Clamp(Matrix values) {

	Vector mins = Vector(min({ values[0][0], values[0][1], values[0][2] }), 
		min({ values[1][0], values[1][1], values[1][2] }),
		min({ values[2][0], values[2][1], values[2][2] }));
	Vector maxs = Vector(max({ values[0][0], values[0][1], values[0][2] }),
		max({ values[1][0], values[1][1], values[1][2] }),
		max({ values[2][0], values[2][1], values[2][2] }));

	Clamp(mins, maxs);
}

float Vector::GetMin() {
	return min({ x, y, z });
}

float Vector::GetMax() {
	return max({ x, y, z });
}

Vector Vector::Reflect(Vector normal) {

	Vector v = (*this);
	return normal * (-2.0f * (normal * v)) + v;
}

Vector Vector::BIntrp(Vector c00, Vector c01, Vector c10, Vector c11) {
	
	return c00 * (1 - x) * (1 - y) + c10 * x * (1 - y) + c01 * (1 - x) * y + c11 * x * y;
}

#pragma endregion Preforms vector operations

#pragma region Graphics Operations

void Vector::SetFromColor(unsigned int color) {

	unsigned char* rgb = (unsigned char*)&color;
	Vector& v = *this;

	v[0] = (float)(rgb[0]) / 255.0f;
	v[1] = (float)(rgb[1]) / 255.0f;
	v[2] = (float)(rgb[2]) / 255.0f;

}

unsigned int Vector::GetColor() {

	unsigned int ret;
	Vector& v = *this;
	unsigned char rgb[3];

	rgb[0] = (unsigned char)(255.0f * v[0]);
	rgb[1] = (unsigned char)(255.0f * v[1]);
	rgb[2] = (unsigned char)(255.0f * v[2]);

	ret = 0xFF000000 + rgb[2] * 256 * 256 + rgb[1] * 256 + rgb[0];

	return ret;
}

#pragma endregion

#pragma region Private Methods

#pragma endregion
