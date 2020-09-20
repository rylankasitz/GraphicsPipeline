#pragma once

#include "framebuffer.h"

class PPC;
class TMesh;

class WorldView {
public:
	PPC* ppc;
	FrameBuffer* fb;

	WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label);
	~WorldView();

	void Render(TMesh* meshes, int meshCount, WorldView** wvs, int wvCount);
	void Show();

private:

};