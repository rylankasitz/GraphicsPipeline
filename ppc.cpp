#include "stdafx.h"

#include "ppc.h"
#include "matrix.h"

#define _USE_MATH_DEFINES 
#include "math.h"

#include <iostream>
#include <fstream>
#include <string>

PPC::PPC(float hfov, int _w, int _h) {

	w = _w;
	h = _h;

	C = Vector::ZERO;
	a = Vector::XAXIS;
	b = Vector::YAXIS * -1.0f;

	float hfovd = hfov / 180.0f * 3.14159236f;

	c = Vector(-(float) w / 2.0f, (float) h / 2, -(float) w / (2 * tanf(hfovd / 2.0f)));

}

#pragma region Projection

int PPC::Project(Vector point, Vector& pointref) {

	Matrix m;

	m.SetColumn(0, a);
	m.SetColumn(1, b);
	m.SetColumn(2, c);

	Vector q = m.Inverted() * (point - C);
	float w = q[2];

	if (w <= 0.0f)
		return 0;

	pointref[0] = q[0] / q[2];
	pointref[1] = q[1] / q[2];
	pointref[2] = 1.0f / w;

	return 1;
}

Vector PPC::UnProject(Vector point) {

	return C + (a * point[0] + b * point[1] + c) / point[2];
}

#pragma endregion

#pragma region Camera Movement

void PPC::TranslateRight(float step) {

	Vector rightDir = a.Normalized();
	C = C + rightDir * step;
}

void PPC::TranslateForward(float step) {

	Vector forwDir = (a ^ b).Normalized();
	C = C + forwDir * step;
}

void PPC::TranslateUp(float step) {

	Vector upDir = b.Normalized() * -1.0f;
	C = C + upDir * step;
}

void PPC::PanRight(float step) {

	Vector adir = b.Normalized();
	a.RotateVector(adir, step);
	c.RotateVector(adir, step);
}

void PPC::TiltUp(float step) {

	Vector bdir = a.Normalized();
	b.RotateVector(bdir, step);
	c.RotateVector(bdir, step);
}

void PPC::RollLeft(float step) {

	Vector cdir = (a ^ b).Normalized();
	a.RotateVector(cdir, step);
	b.RotateVector(cdir, step);
	c.RotateVector(cdir, step);
}

void PPC::SetPose(Vector center, Vector lookAtPoint, Vector upGuidance) {

	Vector newvd = (lookAtPoint - center).Normalized();
	Vector newa = (newvd ^ upGuidance).Normalized();
	Vector newb = (newvd ^ newa).Normalized();

	Vector newc = newvd * GetFocalLength() - newa * (float)w / 2.0f - newb * (float)h / 2.0f;

	a = newa; b = newb; c = newc; C = center;
}

void PPC::Interpolate(PPC* ppcS, PPC* ppcF, int i, int n) {

	float scf = (float)i / (float)(n - 1);
	Vector newC = ppcS->C + (ppcF->C - ppcS->C) * scf;

	Vector sLookAtPoint = ppcS->GetViewDirection() + ppcS->C;
	Vector fLookAtPoint = ppcF->GetViewDirection() + ppcF->C;
	Vector lookAtPoint = sLookAtPoint + (fLookAtPoint - sLookAtPoint) * scf;

	Vector upGuidance = ppcS->b * -1.0f + (ppcF->b * -1.0f - ppcS->b * -1.0f) * scf;
	
	SetPose(newC, lookAtPoint, upGuidance);
}

#pragma endregion

#pragma region Camera Property Methods

void PPC::SetFocalLength(float focalLength) {

	c = GetViewDirection() * focalLength - a * (float)w / 2.0f - b * (float)h / 2.0f;
}

float PPC::GetFocalLength() {

	return GetViewDirection() * c;
}

Vector PPC::GetViewDirection() {

	return (a ^ b).Normalized();
}

#pragma endregion

#pragma region Camera Visualization

void PPC::Visualize(WorldView *wv, float vf) {
	
	unsigned int color = 0xFF000000;
	Vector c1; c1.SetFromColor(color);

	wv->fb->Draw3DPoint(C, wv->ppc, color, 7);

	Vector imageCorners[4];
	float _w = GetFocalLength() / vf;

	imageCorners[0] = UnProject(Vector(0.0f, 0.0f, _w));
	imageCorners[1] = UnProject(Vector(0.0f, (float)h, _w));
	imageCorners[2] = UnProject(Vector((float)w, (float)h, _w));
	imageCorners[3] = UnProject(Vector((float)w, 0.0f, _w));

	Vector c0; c0.SetFromColor(0xFF0000FF);

	for (int i = 0; i < 4; i++) {
		wv->fb->Draw3dSegment(imageCorners[i], imageCorners[(i + 1) % 4], wv->ppc, c0, c0);
	}

	wv->fb->Draw3dSegment(C, imageCorners[0], wv->ppc, c1, c0);
}

#pragma endregion

#pragma region Save/Load

void PPC::Save(const char* textfile) {

	ofstream file (textfile);
	if (file.is_open()) {
		file << a << "\n" << b << "\n" << c << "\n" << C << endl;
		file.close();
	}
	else {
		cerr << "Unable to save file " << textfile << endl;
	}
}


void PPC::Load(const char* textfile) {

	string line;
	ifstream file(textfile);
	if (file.is_open()) {
		
		file >> a; file >> b; file >> c; file >> C;

		file.close();
	}
}

#pragma endregion
