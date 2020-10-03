#include "stdafx.h"

#include <fstream>
#include <iostream>

#include "TMesh.h"
#include "matrix.h"
#include "pointlight.h"

void TMesh::Allocate(int _vertsN, int _trisN) {

	verticesN = _vertsN;
	trisN = _trisN;
	vertices = new Vector[verticesN];
	tris = new unsigned int[trisN * 3];
	BoundingBox = BBox(Vector::ZERO);
}

#pragma region Rendering Methods

void TMesh::DrawMesh(WorldView* wv, PointLight* pl) {

	if (DrawMode == DrawMode::WireFrame) {
		drawWireFrame(wv->fb, wv->ppc);
	}
	else if (DrawMode == DrawMode::Filled) {
		drawFilled(wv->fb, wv->ppc, pl);
	}
}

#pragma endregion

#pragma region Transformations

Vector TMesh::GetCenter() {

	return center;
}

void TMesh::SetCenter(Vector center) {

	Translate(center - this->center);
}

void TMesh::SetScale(float size) {

	for (int i = 0; i < verticesN; i++) {
		vertices[i] = vertices[i] - center;
		vertices[i] = vertices[i] * (size / BoundingBox.Size);
		vertices[i] = vertices[i] + center;
	}

	BoundingBox.Size = size;
}

float TMesh::GetScale() {

	return BoundingBox.Size;
}

void TMesh::Translate(Vector translation) {

	for (int i = 0; i < verticesN; i++) {
		vertices[i] = vertices[i] + translation;
	}

	center = center + translation;
}

void TMesh::Rotate(Vector origin, Vector axis, float angle) {

	for (int i = 0; i < verticesN; i++) {
		vertices[i].RotatePoint(origin, axis, angle);
	}
}

#pragma endregion

#pragma region Set to 3d Objects

void TMesh::SetToCube(Vector cc, float sideLength, unsigned int color1, unsigned int color2) {
	verticesN = 8;
	trisN = 6 * 2;
	Allocate(verticesN, trisN);

	for (int vi = 0; vi < 4; vi++) {
		colors[vi].SetFromColor(color1);
		colors[vi + 4].SetFromColor(color2);
	}

	int vi = 0;
	vertices[vi] = cc + Vector(-sideLength / 2.0f, +sideLength / 2.0f, +sideLength / 2.0f);
	vi++;
	vertices[vi] = cc + Vector(-sideLength / 2.0f, -sideLength / 2.0f, +sideLength / 2.0f);
	vi++;
	vertices[vi] = cc + Vector(+sideLength / 2.0f, -sideLength / 2.0f, +sideLength / 2.0f);
	vi++;
	vertices[vi] = cc + Vector(+sideLength / 2.0f, +sideLength / 2.0f, +sideLength / 2.0f);
	vi++;

	vertices[vi] = vertices[vi - 4] + Vector(0.0f, 0.0f, -sideLength);
	vi++;
	vertices[vi] = vertices[vi - 4] + Vector(0.0f, 0.0f, -sideLength);
	vi++;
	vertices[vi] = vertices[vi - 4] + Vector(0.0f, 0.0f, -sideLength);
	vi++;
	vertices[vi] = vertices[vi - 4] + Vector(0.0f, 0.0f, -sideLength);
	vi++;

	int tri = 0;
	tris[3 * tri + 0] = 0;
	tris[3 * tri + 1] = 1;
	tris[3 * tri + 2] = 2;
	tri++;
	tris[3 * tri + 0] = 2;
	tris[3 * tri + 1] = 3;
	tris[3 * tri + 2] = 0;
	tri++;
	tris[3 * tri + 0] = 3;
	tris[3 * tri + 1] = 2;
	tris[3 * tri + 2] = 6;
	tri++;
	tris[3 * tri + 0] = 6;
	tris[3 * tri + 1] = 7;
	tris[3 * tri + 2] = 3;
	tri++;
	tris[3 * tri + 0] = 7;
	tris[3 * tri + 1] = 6;
	tris[3 * tri + 2] = 5;
	tri++;
	tris[3 * tri + 0] = 5;
	tris[3 * tri + 1] = 4;
	tris[3 * tri + 2] = 7;
	tri++;
	tris[3 * tri + 0] = 4;
	tris[3 * tri + 1] = 5;
	tris[3 * tri + 2] = 1;
	tri++;
	tris[3 * tri + 0] = 1;
	tris[3 * tri + 1] = 0;
	tris[3 * tri + 2] = 4;
	tri++;
	tris[3 * tri + 0] = 4;
	tris[3 * tri + 1] = 0;
	tris[3 * tri + 2] = 3;
	tri++;
	tris[3 * tri + 0] = 3;
	tris[3 * tri + 1] = 7;
	tris[3 * tri + 2] = 4;
	tri++;
	tris[3 * tri + 0] = 1;
	tris[3 * tri + 1] = 5;
	tris[3 * tri + 2] = 6;
	tri++;
	tris[3 * tri + 0] = 6;
	tris[3 * tri + 1] = 2;
	tris[3 * tri + 2] = 1;
	tri++;
}

#pragma endregion

#pragma region Load/Write From File

void TMesh::LoadBin(char* fname) {

	ifstream ifs(fname, ios::binary);

	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	ifs.read((char*)&verticesN, sizeof(int));

	char yn;
	ifs.read(&yn, 1); // vertices 3 floats

	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}

	if (vertices) delete vertices;
	vertices = new Vector[verticesN];

	ifs.read(&yn, 1); // cols 3 floats

	if (colors) delete colors;
	colors = 0;
	if (yn == 'y') {
		colors = new Vector[verticesN];
	}

	ifs.read(&yn, 1); // normals 3 floats

	if (normals) delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new Vector[verticesN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats

	float* tcs = 0; // don't have texture coordinates for now
	if (tcs) delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[verticesN * 2];
	}

	ifs.read((char*)vertices, verticesN * 3 * sizeof(float)); // load verts

	if (colors)
		ifs.read((char*)colors, verticesN * 3 * sizeof(float)); // load cols

	if (normals)
		ifs.read((char*)normals, verticesN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, verticesN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int)); // triangles

	if (tris) delete tris;
	tris = new unsigned int[trisN * 3];

	ifs.read((char*)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	computeBBox();
	computeCenter();

	cerr << "INFO: loaded " << verticesN << " verts, " << trisN << " tris with size of " << BoundingBox.Size << " from " << endl;
	cerr << "      " << fname << endl;
	cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;
}

#pragma endregion

#pragma region Private Methods Drawing

void TMesh::drawCubeQuadFaces(FrameBuffer* fb, PPC* ppc, unsigned int color) {

	Vector c;
	c.SetFromColor(color);
	for (int i = 0; i < 4; i++) {
		fb->Draw3dSegment(vertices[i], vertices[(i + 1) % 4], ppc, c, c);
		fb->Draw3dSegment(vertices[4 + i], vertices[4 + (i + 1) % 4], ppc, c, c);
		fb->Draw3dSegment(vertices[i], vertices[i + 4], ppc, c, c);
	}
}

void TMesh::drawWireFrame(FrameBuffer* fb, PPC* ppc) {

	for (int i = 0; i < trisN; i++) {
		Vector v1 = vertices[tris[3 * i + 0]];
		Vector v2 = vertices[tris[3 * i + 1]];
		Vector v3 = vertices[tris[3 * i + 2]];

		Vector c1 = colors[tris[3 * i + 0]];
		Vector c2 = colors[tris[3 * i + 1]];
		Vector c3 = colors[tris[3 * i + 2]];

		fb->Draw3dSegment(v1, v2, ppc, c1, c2);
		fb->Draw3dSegment(v2, v3, ppc, c2, c3);
		fb->Draw3dSegment(v3, v1, ppc, c3, c1);
	}
}

#pragma region Unused

void TMesh::drawFilled_Unused(FrameBuffer* fb, PPC* ppc) {

	Vector* pverts = new Vector[verticesN];
	for (int vi = 0; vi < verticesN; vi++) {
		if (!ppc->Project(vertices[vi], pverts[vi]))
			pverts[vi] = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	for (int tri = 0; tri < trisN; tri++) {
		unsigned int vinds[3] = { tris[3 * tri + 0], tris[3 * tri + 1], tris[3 * tri + 2] };

		if (pverts[vinds[0]][0] == FLT_MAX ||
			pverts[vinds[1]][0] == FLT_MAX ||
			pverts[vinds[2]][0] == FLT_MAX)
			continue;

		BBox bbox(pverts[vinds[0]]);
		bbox.AddPoint(pverts[vinds[1]]);
		bbox.AddPoint(pverts[vinds[2]]);

		if (!bbox.ClipWithFrame(fb->w, fb->h))
			continue;

		int left = (int)(bbox.Corners[0][0] + .5f);
		int right = (int)(bbox.Corners[1][0] - .5f);
		int top = (int)(bbox.Corners[0][1] + .5f);
		int bottom = (int)(bbox.Corners[1][1] - .5f);

		Matrix vertsm(pverts[vinds[0]], pverts[vinds[1]], pverts[vinds[2]]);

		float minZ = vertsm.GetColumn(2).GetMin();
		float maxZ = vertsm.GetColumn(2).GetMax();

		Matrix cm = Matrix(colors[vinds[0]], colors[vinds[1]], colors[vinds[2]]).Transposed();
		Matrix eeqsm = vertsm.GetEdgeEQS();
		Matrix ssim = vertsm.GetSSIM();

		Matrix cme;
		cme[0] = ssim * cm[0];
		cme[1] = ssim * cm[1];
		cme[2] = ssim * cm[2];

		Vector ze = ssim * Vector(pverts[vinds[0]][2], pverts[vinds[1]][2], pverts[vinds[2]][2]);

		for (int v = top; v <= bottom; v++) {
			for (int u = left; u <= right; u++) {
				Vector currentPix(.5f + (float)u, .5f + (float)v, 1.0f);
				Vector side = eeqsm * currentPix;

				if (side[0] < 0.0f || side[1] < 0.0f || side[2] < 0.0f)
					continue; // outside of triangle

				float currentZ = ze * currentPix;
				if (fb->Farther(u, v, clamp(currentZ, minZ, maxZ)))
					continue; // behind another triangle

				Vector color = cme * currentPix;
				color.Clamp(cm);
				fb->Set(u, v, color.GetColor());
			}
		}
	}

	delete[]pverts;
}


#pragma endregion

void TMesh::drawFilled(FrameBuffer* fb, PPC* ppc, PointLight* pl) {

	Vector* pverts = new Vector[verticesN];
	for (int vi = 0; vi < verticesN; vi++) {
		if (!ppc->Project(vertices[vi], pverts[vi]))
			pverts[vi] = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	for (int tri = 0; tri < trisN; tri++) {
		unsigned int vinds[3] = { tris[3 * tri + 0], tris[3 * tri + 1], tris[3 * tri + 2] };

		if (pverts[vinds[0]][0] == FLT_MAX ||
			pverts[vinds[1]][0] == FLT_MAX ||
			pverts[vinds[2]][0] == FLT_MAX)
			continue;

		BBox bbox(pverts[vinds[0]]);
		bbox.AddPoint(pverts[vinds[1]]);
		bbox.AddPoint(pverts[vinds[2]]);

		if (!bbox.ClipWithFrame(fb->w, fb->h))
			continue;

		int left = (int)(bbox.Corners[0][0] + .5f);
		int right = (int)(bbox.Corners[1][0] - .5f);
		int top = (int)(bbox.Corners[0][1] + .5f);
		int bottom = (int)(bbox.Corners[1][1] - .5f);

		// Verts Matricies
		Matrix vertsP(pverts[vinds[0]], pverts[vinds[1]], pverts[vinds[2]]);
		Matrix vertsUP(vertices[vinds[0]], vertices[vinds[1]], vertices[vinds[2]]);

		// Type Matricies
		Matrix cm = Matrix(colors[vinds[0]], colors[vinds[1]], colors[vinds[2]]).Transposed();
		Matrix nm = Matrix(normals[vinds[0]], normals[vinds[1]], normals[vinds[2]]).Transposed();

		// Equation Matricies
		Matrix pim = vertsUP.GetPIM(ppc).Transposed();
		Matrix eeqsm = vertsP.GetEdgeEQS();

		// Equation variables
		Matrix cme(pim * cm[0], pim * cm[1], pim * cm[2]);
		Matrix nme(pim * nm[0], pim * nm[1], pim * nm[2]);
		Vector ze = pim * Vector(pverts[vinds[0]][2], pverts[vinds[1]][2], pverts[vinds[2]][2]);

		Vector w = pim * Vector::ONES;

		for (int v = top; v <= bottom; v++) {
			for (int u = left; u <= right; u++) {
				Vector currentPix(.5f + (float)u, .5f + (float)v, 1.0f);
				Vector side = eeqsm * currentPix;

				if (side[0] < 0.0f || side[1] < 0.0f || side[2] < 0.0f)
					continue; // outside of triangle

				float currentZ = (ze * currentPix) / (w * currentPix);
				if (fb->Farther(u, v, currentZ))
					continue; // behind another triangle

				// Demonimator
				float d = 1 / (w * currentPix);

				// Normal at current pixel
				Vector normal = ((nme * currentPix) * d).Normalized();	

				// Color at current pixel
				Vector color = (cme * currentPix) * d;

				// Light at current pixel
				Vector currP = ppc->UnProject(Vector(currentPix[0], currentPix[1], currentZ));
				Vector light = (pl->Center - currP).Normalized();

				color = pl->Light(color, light, normal, (ppc->C - currP).Normalized());

				color.Clamp(Vector::ZERO, Vector::ONES);
				fb->Set(u, v, color.GetColor());
			}
		}
	}

	delete[]pverts;
}

#pragma endregion

#pragma region Private Methods Computations

void TMesh::computeBBox() {

	BoundingBox = BBox(vertices[0]);
	for (int i = 0; i < verticesN; i++) {
		BoundingBox.AddPoint(vertices[i]);
	}
}

void TMesh::computeCenter() {
	Vector newCenter(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < verticesN; i++) {
		newCenter = newCenter + vertices[i];
	}

	this->center = newCenter / (float)verticesN;
}

#pragma endregion
