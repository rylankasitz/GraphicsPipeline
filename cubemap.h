#pragma once

#include "texture.h"

class FrameBuffer;
class PPC;
class Vector;
class WorldView;
class Texture;

class CubeMap
{
public:
	PPC* ppcs[6];			// top-left-front-right-back-bottom
	Texture* textures[6]; 

	int lastlookup;

	CubeMap();
	void Load(Texture cubemap);
	Vector Lookup(Vector ruv);

private:
	void setPPC(int w, int h);
};