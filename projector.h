#pragma once

#include "vector.h"

class WorldView;
class Scene;

class Projector
{
public:
	WorldView* wv;
	Scene* scene;
	Vector center, lookatPoint;

	Projector(char* tex, Scene* scene);

	void Render(WorldView* wv);
	void RenderFB();
	bool GetProjectorColor(Vector point, Vector& color);

	void SetPostion(Vector position, Vector lookatpoint);
	void Rotate(Vector origin, Vector direction, float angle);

private:

};