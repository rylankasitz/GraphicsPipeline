#pragma once

#include <iostream>

class Vector {
public:
	static Vector XAXIS;
	static Vector YAXIS;
	static Vector ZAXIS;

	Vector();
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

	void Normalize();
	void RotatePoint(Vector origin, Vector direction, float angle);
	void RotateVector(Vector axis);

	float Length();

private:
	float x, y, z;
};