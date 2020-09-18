#include "stdafx.h"

#include <fstream>
#include <iostream>

#include "TMesh.h"


void TMesh::Allocate(int _vertsN, int _trisN) {

	verticesN = _vertsN;
	trisN = _trisN;
	vertices = new Vector[verticesN];
	tris = new unsigned int[trisN * 3];
	BBox = BBox::BBox();
}

void TMesh::DrawMesh(FrameBuffer* fb, PPC* ppc) {

	if (DrawMode == DrawMode::WireFrame) {
		drawWireFrame(fb, ppc);
	}
	else if (DrawMode == DrawMode::Filled) {
		drawFilled(fb, ppc);
	}
}

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
		vertices[i] = vertices[i] * (size / BBox.Size);
		vertices[i] = vertices[i] + center;
	}

	BBox.Size = size;
}

float TMesh::GetScale() {

	return BBox.Size;
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

	cerr << "INFO: loaded " << verticesN << " verts, " << trisN << " tris with size of " << BBox.Size << " from " << endl;
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

void TMesh::drawFilled(FrameBuffer* fb, PPC* ppc) {

	for (int i = 0; i < trisN; i++) {
		Vector v1 = vertices[tris[3 * i + 0]];
		Vector v2 = vertices[tris[3 * i + 1]];
		Vector v3 = vertices[tris[3 * i + 2]];

		Vector c1 = colors[tris[3 * i + 0]];
		Vector c2 = colors[tris[3 * i + 1]];
		Vector c3 = colors[tris[3 * i + 2]];

		fb->Draw3dTriangle(v1, v2, v3, c1, c2, c3, ppc);
	}
}

#pragma endregion

#pragma region Private Methods Computations

void TMesh::computeBBox() {

	Vector min = vertices[0], max = vertices[0];
	for (int i = 0; i < verticesN; i++) {
		for (int j = 0; j < 3; j++) {
			if (vertices[i][j] < min[j]) {
				min[j] = vertices[i][j];
			}
			if (vertices[i][j] > max[j]) {
				max[j] = vertices[i][j];
			}
		}
	}

	BBox.SetBBox(min, max);
}

void TMesh::computeCenter() {
	Vector newCenter(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < verticesN; i++) {
		newCenter = newCenter + vertices[i];
	}

	this->center = newCenter / (float)verticesN;
}

#pragma endregion
