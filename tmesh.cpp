#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <string>

#include "TMesh.h"
#include "matrix.h"
#include "light.h"
#include "scene.h"
#include "texture.h"


void TMesh::Allocate(int _vertsN, int _trisN) {

	verticesN = _vertsN;
	trisN = _trisN;

	BoundingBox = BBox(Vector::ZERO);
	material = Material::Default;
}

#pragma region Rendering Methods

void TMesh::DrawMesh(WorldView* wv, Scene* scene) {

	if (DrawMode == MeshMode::WireFrame) {
		drawWireFrame(wv->fb, wv->ppc);
	}
	else if (DrawMode == MeshMode::Filled) {
		drawFilled(wv->fb, wv->ppc, scene, wv->renderMode);
	}
}

void TMesh::DrawHW() {

	if (DrawMode == MeshMode::Filled) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_TEXTURE);

	glVertexPointer(3, GL_FLOAT, 0, (float*)verticesArr);
	glColorPointer(3, GL_FLOAT, 0, (float*)colorsArr);
	glNormalPointer(GL_FLOAT, 0, (float*)normalsArr);
	glTexCoordPointer(2, GL_FLOAT, 0, (float*)textcoords);

	glDisable(GL_TEXTURE_2D);
	if (material.texture.textureHandle)
	{
		glEnable(GL_TEXTURE_2D);
		//glActiveTexture(CG_TEXCOORD0);
		glBindTexture(GL_TEXTURE_2D, material.texture.textureHandle);

	}

	glDrawElements(GL_TRIANGLES, trisv.size(), GL_UNSIGNED_INT, trisvArr);

	glDisableClientState(GL_TEXTURE);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
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

	for (int i = 0; i < trisv.size(); i++)
	{
		verticesArr[i] = vertices[trisv[i]];
	}

	computeBBox();

	BoundingBox.Size = size;
}

float TMesh::GetScale() {

	return BoundingBox.Size;
}

void TMesh::Translate(Vector translation) {

	for (int i = 0; i < verticesN; i++) 
	{
		vertices[i] = vertices[i] + translation;
	}

	for (int i = 0; i < trisv.size(); i++)
	{
		verticesArr[i] = vertices[trisv[i]];
	}

	computeBBox();

	center = center + translation;
}

void TMesh::Rotate(Vector origin, Vector axis, float angle) {

	for (int i = 0; i < verticesN; i++) {
		vertices[i].RotatePoint(origin, axis, angle);
	}

	for (int i = 0; i < trisv.size(); i++)
	{
		verticesArr[i] = vertices[trisv[i]];
	}
}

void TMesh::SetBBox(BBox& aabb) {

	for (int vi = 0; vi < vertices.size(); vi++) {
		aabb.AddPoint(vertices[vi]);
	}
}

void TMesh::SetMaterial(Material material) {

	this->material = material;
	for (int i = 0; i < trisv.size(); i++) {
		colorsArr[i] = material.color;
	}
}

#pragma endregion

#pragma region Load/Write From File

void TMesh::LoadObj(char* fname) {

	ifstream ifs(fname, ios::in);

	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	verticesN = 0; trisN = 0;
	for (string line; getline(ifs, line); )
	{
		if (line[0] == '#')
			continue;

		vector<string> pline = split(line, " ");

		if (pline[0] == "v") {
			vertices.push_back(Vector(stof(pline[1]), stof(pline[2]), stof(pline[3])));
			colors.push_back(material.color); // Give it a default color
			verticesN++;
		}
		else if (pline[0] == "vt") {
			textures.push_back(Vector(stof(pline[1]), stof(pline[2]), 0.0f));
		}
		else if (pline[0] == "vn") {
			normals.push_back(Vector(stof(pline[1]), stof(pline[2]), stof(pline[3])));
		}
		else if (pline[0] == "f") {
			for (int p = 0; p < 3; p++) {
				vector<string> m = split(pline[p + 1], "/");

				trisv.push_back(stoi(m[0]) - 1);
				trisvt.push_back(stoi(m[1]) - 1);
				trisvn.push_back(stoi(m[2]) - 1);
			}
			trisN++;
		}
	}

	
	trisvArr = new unsigned int[trisv.size()];
	verticesArr = new Vector[trisv.size()];
	colorsArr = new Vector[trisv.size()];
	textcoords = new float[trisv.size() * 2];
	normalsArr = new Vector[trisv.size()];

	int j = 0;
	for (int i = 0; i < trisv.size(); i++)
	{
		verticesArr[i] = vertices[trisv[i]];
		colorsArr[i] = material.color;
		normalsArr[i] = normals[trisvn[i]];
		trisvArr[i] = i;
		textcoords[j] = textures[trisvt[i]][0];
		textcoords[j + 1] = textures[trisvt[i]][1];
		j += 2;
	}

	computeBBox();
	computeCenter();
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
		Vector v1 = vertices[trisv[3 * i + 0]];
		Vector v2 = vertices[trisv[3 * i + 1]];
		Vector v3 = vertices[trisv[3 * i + 2]];

		Vector c1 = colors[trisv[3 * i + 0]];
		Vector c2 = colors[trisv[3 * i + 1]];
		Vector c3 = colors[trisv[3 * i + 2]];

		fb->Draw3dSegment(v1, v2, ppc, c1, c2);
		fb->Draw3dSegment(v2, v3, ppc, c2, c3);
		fb->Draw3dSegment(v3, v1, ppc, c3, c1);
	}
}

void TMesh::drawFilled(FrameBuffer* fb, PPC* ppc, Scene* scene, RenderMode rmode) {

	Vector* pverts = new Vector[verticesN];
	Vector* pvertsW = new Vector[verticesN];
	for (int vi = 0; vi < verticesN; vi++) {
		if (!ppc->Project(vertices[vi], pverts[vi]))
			pverts[vi] = Vector(FLT_MAX, FLT_MAX, FLT_MAX);

		if (!ppc->ProjectW(vertices[vi], pvertsW[vi]))
			pvertsW[vi] = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	for (int tri = 0; tri < trisN; tri++) {
		unsigned int vinds[3] = { trisv[3 * tri + 0], trisv[3 * tri + 1], trisv[3 * tri + 2] };
		unsigned int vtinds[3] = { trisvt[3 * tri + 0], trisvt[3 * tri + 1], trisvt[3 * tri + 2] };
		unsigned int vninds[3] = { trisvn[3 * tri + 0], trisvn[3 * tri + 1], trisvn[3 * tri + 2] };

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
		Matrix vertsPW(pvertsW[vinds[0]], pvertsW[vinds[1]], pvertsW[vinds[2]]);
		Matrix vertsUP(vertices[vinds[0]], vertices[vinds[1]], vertices[vinds[2]]);

		// Type Matricies
		Matrix cm = Matrix(colors[vinds[0]], colors[vinds[1]], colors[vinds[2]]).Transposed();
		Matrix nm = Matrix(normals[vninds[0]], normals[vninds[1]], normals[vninds[2]]).Transposed();
		Matrix tm = Matrix(textures[vtinds[0]], textures[vtinds[1]], textures[vtinds[2]]).Transposed();

		// Equation Matricies
		Matrix pim = vertsUP.GetPIM(ppc).Transposed();
		Matrix eeqsm = vertsP.GetEdgeEQS();

		// Equation variables
		Matrix cme(pim * cm[0], pim * cm[1], pim * cm[2]);
		Matrix nme(pim * nm[0], pim * nm[1], pim * nm[2]);
		Matrix tme(pim * tm[0], pim * tm[1], pim * tm[2]);
		Vector ze = pim * vertsPW.GetColumn(2);

		Vector w = pim * Vector::ONES;

		for (int v = top; v <= bottom; v++) {
			for (int u = left; u <= right; u++) {
				Vector currentPix(.5f + (float)u, .5f + (float)v, 1.0f);
				Vector side = eeqsm * currentPix;

				if (side[0] < 0.0f || side[1] < 0.0f || side[2] < 0.0f)
					continue; // outside of triangle

				// Demonimator
				float d = 1 / (w * currentPix);

				float currentZ = (w * currentPix) / (ze * currentPix);
				if (fb->Farther(u, v, currentZ))
					continue; // behind another triangle

				if (rmode == RenderMode::ShadowMap)
					continue; // only get zbuffer for shadow map

				// Get unporjected pixel
				Vector currP = ppc->UnProject(Vector(currentPix[0], currentPix[1], currentZ));

				// Normal, color, texture, light at current pixel
				Vector normal = ((nme * currentPix) * d).Normalized();	
				Vector color = (cme * currentPix) * d;
				Vector texture = (tme * currentPix) * d;
				Vector light = (scene->plight->center - currP).Normalized();

				// Add reflection
				if (material.reflection)
				{
					// Calculate reflection
					Vector euv = (currP - ppc->C).Normalized();
					Vector ruv = euv.Reflect(normal);
					color = scene->envmap->cubemap.Lookup(ruv);
				}
				else 
				{	
					// Add lighting to pixel and color	
					material.SetPix(texture[0], texture[1]);
					color = scene->plight->GetColor(light, normal, (ppc->C - currP).Normalized(), material);

					// Add shadow to pixel
					if (scene->plight->InShadow(currP)) {
						color = color * scene->plight->ambience;
					}
				}

				// Add projector to pixel
				/*Vector projColor;
				if (scene->projector->GetProjectorColor(currP, projColor)) {
					color = projColor;
				}*/	

				fb->Set(u, v, color.GetColor());
			}
		}
	}

	delete[]pverts;
	delete[]pvertsW;
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

#pragma region Helpermethods

vector<string> TMesh::split(string str, string del) {

	int pos = 0, count = 0;
	vector<string> tokens;
	while ((pos = str.find(del)) != std::string::npos) {
		string s = str.substr(0, pos);
		str.erase(0, pos + del.length());
		if (s == "") continue;
		tokens.push_back(s);
		count++;
	}
	tokens.push_back(str);

	return tokens;
}

#pragma endregion