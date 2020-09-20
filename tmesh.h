#pragma once

#include "ppc.h"
#include "framebuffer.h"
#include "bbox.h";

enum class DrawMode { WireFrame, Filled };

class TMesh {
public:

	#pragma region Properties

	DrawMode DrawMode = DrawMode::Filled;
	BBox BBox;

	#pragma endregion

	TMesh() : vertices(0), colors(0), normals(0), verticesN(0), tris(0), trisN(0) {};
	void Allocate(int _vertsN, int _trisN);

	void DrawMesh(FrameBuffer* fb, PPC* ppc);
	void SetToCube(Vector cc, float sideLength, unsigned int color1, unsigned int color2);

	void SetCenter(Vector center);
	Vector GetCenter();
	void SetScale(float size);
	float GetScale();
	void Translate(Vector translation);
	void Rotate(Vector origin, Vector axis, float angle);

	void LoadBin(char* fname);

private: 
	Vector* vertices;
	Vector* colors;
	Vector* normals;

	Vector center;

	int verticesN;
	unsigned int* tris;
	int trisN;

	void drawCubeQuadFaces(FrameBuffer* fb, PPC* ppc, unsigned int color);
	void drawWireFrame(FrameBuffer* fb, PPC* ppc);
	void drawFilled(FrameBuffer* fb, PPC* ppc);
	void computeBBox();
	void computeCenter();
};