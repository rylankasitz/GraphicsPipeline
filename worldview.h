#pragma once

#include "framebuffer.h"
#include <vector>
#include "CGInterface.h"
#include <GL/gl.h>

class PPC;
class TMesh;
class Light;
class Scene;

enum class RenderMode { Full, ShadowMap, CubeMap, SceneRender };
enum class DrawMode { CPU, HardWare, GPU };

class WorldView {
public:
	PPC* ppc;
	FrameBuffer* fb;
	RenderMode renderMode;
	DrawMode drawMode;
	CGInterface* cgi;
	ShaderOneInterface* soi;
	Scene* scene;

	WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label, Scene * scene);
	~WorldView();

	void Render(Scene* scene);
	void RenderCPU(Scene* scene);
	void RenderHW(Scene* scene);
	void RenderGPU(Scene* scene);
	void RenderMeshes(Scene* scene);

	void Show();

private:

};