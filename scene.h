#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "vector.h"
#include "matrix.h"
#include "tmesh.h"
#include "ppc.h"
#include "worldview.h"
#include "inputhandler.h";
#include "pointlight.h"

class Scene {
public:

	GUI *gui;
	WorldView** views;
	WorldView *worldView;
	TMesh *meshes;
	PointLight* plight;

	int meshCount;
	int viewCount;

	Scene();

	void Load();
	void DBG();
	void Demo();
	void Render();
};

extern Scene *scene;