#pragma once

class Vector;
class WorldView;
class Scene;

class Projector
{
public:
	WorldView* wv;
	Projector(char* tex, Vector position, Vector lookatpoint);

	void RenderFB(Scene* scene);
	bool GetProjectorColor(Vector point, Vector& color);

private:

};