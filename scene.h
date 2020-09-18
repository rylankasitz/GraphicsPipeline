#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "vector.h"
#include "matrix.h"
#include "tmesh.h"
#include "ppc.h"

class Scene {
public:

	GUI *gui;
	FrameBuffer *fb;
	TMesh *meshes;
	PPC *mainCamera;
	PPC** cameras;

	int meshCount;
	int cameraCount;

	Scene();

	void Load();
	void DBG();
	void Demo();
	void Render();
};

extern Scene *scene;