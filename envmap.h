#pragma once
#include "cubemap.h"

class Vector;
class WorldView;

class EnvMap {
public:
	CubeMap cubemap;

	EnvMap(CubeMap _cubemap);
	void Render(WorldView* wv);

private:

};