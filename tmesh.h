#pragma once

#include "ppc.h"
#include "framebuffer.h"
#include "bbox.h";
#include "material.h";

#include <vector>

class Light;
class Matrix;
class Material;

enum class DrawMode { WireFrame, Filled };

class TMesh {
public:

	#pragma region Properties

	DrawMode DrawMode = DrawMode::Filled;
	BBox BoundingBox;
	Material Material;

	#pragma endregion

	TMesh() : vertices(0), colors(0), normals(0), textures(0), 
		trisv(0), trisvt(0), trisvn(0), 
		verticesN(0), texturesN(0), trisN(0), 
		BoundingBox(Vector::ZERO), Material(0xFF0000FF, 32) {};

	void Allocate(int _vertsN, int _trisN);

	void DrawMesh(WorldView* wv, Light* pl);

	void SetCenter(Vector center);
	Vector GetCenter();
	void SetScale(float size);
	float GetScale();
	void Translate(Vector translation);
	void Rotate(Vector origin, Vector axis, float angle);

	void LoadObj(char* fname);
	void LoadObj(char* fname, char* tname, bool tiled);

private: 
	vector<Vector> vertices; vector<Vector> colors; vector<Vector> normals; vector<Vector> textures;

	Vector center;

	int verticesN; int texturesN; int trisN;
	vector<unsigned int> trisv; vector<unsigned int> trisvt; vector<unsigned int> trisvn;

	void drawCubeQuadFaces(FrameBuffer* fb, PPC* ppc, unsigned int color);
	void drawWireFrame(FrameBuffer* fb, PPC* ppc);
	void drawFilled(FrameBuffer* fb, PPC* ppc, Light* pl);

	void computeBBox();
	void computeCenter();

	vector<string> split(string str, string del);
	
};