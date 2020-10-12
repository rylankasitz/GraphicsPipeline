#pragma once

#include "vector.h"
#include <vector>

class Material;
class WorldView;
class TMesh;
class Scene;

class Light
{
public:
	Vector center;
	float ambience, intensity;
	WorldView* wvShadowMap;
	Scene* scene;

	Light();
	Light(float ambient, float intensity, Scene* scene);

	void Render(WorldView* wv);
	Vector GetColor(Vector lightv, Vector normalv, Vector vd, Material m);
	void RenderShadowMap();
	bool InShadow(Vector point);

	void SetPosition(Vector position);
	void Rotate(Vector origin, Vector direction, float angle);

private:
};