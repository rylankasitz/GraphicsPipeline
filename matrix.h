#pragma once

#include "vector.h"

class Matrix {
public:
	Matrix() {};
	Matrix(Vector _r0, Vector _r1, Vector _r2);

	const Vector& operator[](const int i) const;
	Vector& operator[](const int i);
	Vector operator*(Vector v);
	Matrix operator*(Matrix m);

	friend std::ostream& operator<<(std::ostream&, const Matrix);

	Vector GetColumn(int i);

	void SetColumn(int i, Vector v);
	void Invert();
	void Transpose();
	void SetRotationMatrix(float degrees);

private:
	Vector r0, r1, r2;
	int rows, cols;

	float getCofactor(int row, int col);
};