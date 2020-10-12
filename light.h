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
	float ambience;
	float intensity;
	WorldView* wvShadowMap;

	Light();
	Light(Vector center, float ambient, float intensity);

	void Render(WorldView* wv);
	Vector GetColor(Vector lightv, Vector normalv, Vector vd, Material m);
	void RenderShadowMap(Scene* scene);
	bool InShadow(Vector point);

private:
};