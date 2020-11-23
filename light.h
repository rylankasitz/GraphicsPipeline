#pragma once

#include "vector.h"
#include <vector>

class Material;
class WorldView;
class TMesh;
class Scene;
class BBox;

class Light
{
public:
	Vector center;
	float ambience, intensity;
	WorldView* wvShadowMap;
	Scene* scene;
	Vector areaV[4];
	float size = 100;

	GLuint depthMapFBO;
	GLuint depthMap;

	Light();
	Light(float ambient, float intensity, Scene* scene);

	void Load();
	void Render(WorldView* wv);
	Vector GetColor(Vector lightv, Vector normalv, Vector vd, Material m);
	void RenderShadowMap();
	bool InShadow(Vector point);

	void SetPosition(Vector position);
	void Rotate(Vector origin, Vector direction, float angle);

private:
};