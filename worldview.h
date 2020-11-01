#pragma once

#include "framebuffer.h"
#include <vector>

class PPC;
class TMesh;
class Light;
class Scene;

enum class RenderMode { Full, ShadowMap, CubeMap, SceneRender };

class WorldView {
public:
	PPC* ppc;
	FrameBuffer* fb;
	RenderMode renderMode;

	WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label);
	~WorldView();

	void Render(Scene* scene);
	void Show();

private:

};