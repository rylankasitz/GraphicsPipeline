#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "vector.h"
#include "matrix.h"
#include "tmesh.h"
#include "ppc.h"
#include "worldview.h"
#include "inputhandler.h";
#include "light.h"
#include "projector.h"
#include "resourcemanager.h"
#include "envmap.h"

class Scene {
public:

	GUI *gui;
	ResourceManager* rm;
	WorldView *worldView;

	vector<WorldView*> views;

	Light* plight;
	Projector* projector;
	EnvMap* envmap;

	float p1; // test parameter for fragment shader

	Scene();

	void Load();
	void DBG();
	void Demo();
	void Render();
	void shadowDemo();
	void hDemo();
};

extern Scene *scene;