#pragma once

#include "worldview.h"
#include "vector.h"

class PPC {
public:
	PPC(float horizontalFOV, int _w, int _h);

	int Project(Vector point, Vector& pointref);
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

	void Visualize(WorldView* wv, float vf);

private:
	Vector a, b, c, C;
	int w, h;
};