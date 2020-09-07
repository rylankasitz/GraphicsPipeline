#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "vector.h"
#include "matrix.h"


class Scene {
public:

	GUI *gui;
	FrameBuffer *fb;
	Scene();
	void DBG();
	void Calc();
	void Demo();
};

extern Scene *scene;