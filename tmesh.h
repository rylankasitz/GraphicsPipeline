#pragma once

#include "ppc.h"
#include "framebuffer.h"
#include "bbox.h";
#include "material.h";

#include <vector>
#include "resourcemanager.h"

class Light;
class Matrix;
class Material;
class Scene;

enum class MeshMode { WireFrame, Filled };

class TMesh {
public:

	#pragma region Properties

	MeshMode DrawMode = MeshMode::Filled;
	BBox BoundingBox;
	Material material;
	int id = -1;
	Vector* verticesArr;
	vector<Vector> vertices;
	vector<unsigned int> trisv;
	Vector* colorsArr;

	#pragma endregion

	TMesh() : vertices(0), colors(0), normals(0), textures(0), 
		trisv(0), trisvt(0), trisvn(0), 
		verticesN(0), texturesN(0), trisN(0), 
		BoundingBox(Vector::ZERO), material(CR_RED, false) {};

	void Allocate(int _vertsN, int _trisN);

	void DrawMesh(WorldView* wv, Scene* scene);
	void DrawHW();

	void SetCenter(Vector center);
	Vector GetCenter();
	void SetScale(float size);
	float GetScale();
	void Translate(Vector translation);
	void Rotate(Vector origin, Vector axis, float angle);
	void SetBBox(BBox& aabb);
	void SetMaterial(Material material);

	void LoadObj(char* fname);

private: 
	vector<Vector> colors; vector<Vector> normals; vector<Vector> textures;

	Vector center;

	int verticesN; int texturesN; int trisN;
	vector<unsigned int> trisvt; vector<unsigned int> trisvn;

	Vector* normalsArr;
	float* textcoords;
	unsigned int *trisvArr;
	

	void drawCubeQuadFaces(FrameBuffer* fb, PPC* ppc, unsigned int color);
	void drawWireFrame(FrameBuffer* fb, PPC* ppc);
	void drawFilled(FrameBuffer* fb, PPC* ppc, Scene* scene, RenderMode rmode);

	void computeBBox();
	void computeCenter();

	vector<string> split(string str, string del);
	
};