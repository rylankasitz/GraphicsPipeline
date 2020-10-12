#pragma once

#include "worldview.h"
#include "vector.h"

class Matrix;

class PPC {
public:
	Vector a, b, c, C;

	PPC(float horizontalFOV, int _w, int _h);

	int Project(Vector point, Vector& pointref);
	int ProjectW(Vector point, Vector& pointref);
	Vector UnProject(Vector point);

	void TranslateRight(float step);
	void TranslateForward(float step);
	void TranslateUp(float step);

	void PanRight(float step);
	void TiltUp(float step);
	void RollLeft(float step);

	void SetPose(Vector center, Vector lookAtPoint, Vector upGuidance);
	void Interpolate(PPC* ppcS, PPC* ppcF, int i, int n);

	void SetFocalLength(float focalLength);
	float GetFocalLength();
	Vector GetViewDirection();
	Matrix GetPerspectiveM();

	void Visualize(WorldView* wv, float vf);

	void Save(const char* textfile);
	void Load(const char* textfile);

private:
	int w, h;
};