#pragma once

#include <iostream>

using namespace std;

float clamp(float value, float min, float max);

class Vector {
public:
	static Vector XAXIS;
	static Vector YAXIS;
	static Vector ZAXIS;
	static Vector ZERO;

	Vector() {};
	Vector(float x, float y, float z);

	const float& operator[](const int i) const;
	float& operator[](const int i);
	Vector operator+(Vector v);
	Vector operator-(Vector v);
	float operator*(Vector v);
	Vector operator^(Vector v);
	Vector operator*(float s);
	Vector operator/(float s);

	friend std::ostream& operator<<(std::ostream&, const Vector);
	friend std::istream& operator>>(std::istream&, Vector&);

	void Normalize();
	Vector Normalized();
	void RotatePoint(Vector origin, Vector direction, float angle);
	void RotateVector(Vector axis, float angle);
	void Clamp(Vector mins, Vector maxs);

	void SetFromColor(unsigned int color);
	unsigned int GetColor();

	float Length();

private:
	float x, y, z;
};