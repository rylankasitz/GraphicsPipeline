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
	friend std::istream& operator>>(std::istream&, Matrix&);

	Vector GetColumn(int i);
	Matrix Inverted();
	Matrix Transposed();
	void SetColumn(int i, Vector v);
	void Invert();
	void Transpose();
	void SetRotationMatrixX(float radians);
	void SetRotationMatrixY(float radians);
	void SetRotationMatrixZ(float radians);

	Matrix GetEdgeEQS();
	Matrix GetSSIM();

private:
	Vector r0, r1, r2;
	int rows, cols;

	Vector getEdgeEQ(Vector v1, Vector v2, Vector v3);
};