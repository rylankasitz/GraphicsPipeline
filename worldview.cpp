#include "stdafx.h"
#include "worldview.h"
#include "tmesh.h"
#include "ppc.h"
#include "light.h"
#include "scene.h"

WorldView::WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label, Scene * scene) {

	fb = new FrameBuffer(x, y, w, h, id);
	fb->label(label);
	fb->wv = this;
	this->scene = scene;

	drawMode = DrawMode::CPU;

	ppc = new PPC(hfov, fb->w, fb->h);
	renderMode = RenderMode::Full;
}

WorldView::~WorldView() {
	delete fb;
	delete ppc;
}

void WorldView::Show() {
	fb->show();
	fb->redraw();
}

void WorldView::Render(Scene* scene)
{
	fb->redraw();
}

void WorldView::RenderCPU(Scene* scene) {

	fb->ClearZB();
	if (renderMode == RenderMode::Full || renderMode == RenderMode::SceneRender) {
		scene->envmap->Render(this);
	}

	if (renderMode == RenderMode::Full) {
		for (auto& wv : scene->views) {
			wv->ppc->Visualize(this, 20.0f);
		}
	}

	for (auto& mesh : scene->rm->meshes) {
		mesh.second.DrawMesh(this, scene);
	}

	scene->plight->Render(this);
	scene->projector->Render(this);
}

void WorldView::RenderHW(Scene * scene) {

	// initializations (could be done once per session)
	glEnable(GL_DEPTH_TEST);

	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// set the view desired by the application (the user)
	ppc->SetIntrinsicsHW();
	ppc->SetExtrinsicsHW();

	// draw the actual geometry
	for (auto& mesh : scene->rm->meshes) {
		mesh.second.DrawHW();
	}
}

void WorldView::RenderGPU(Scene * scene) {

	// if the first time, call per session initialization
	if (cgi == NULL) {
		scene->plight->Load();
		scene->rm->Load();

		cgi = new CGInterface();
		cgi->PerSessionInit();
		soi = new ShaderOneInterface(scene);
		soi->PerSessionInit(cgi);
	}

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderMeshes(scene);
}

void WorldView::RenderMeshes(Scene* scene)
{
	// set the view desired by the application (the user)
	ppc->SetIntrinsicsHW();
	ppc->SetExtrinsicsHW();

	// per frame initialization
	cgi->EnableProfiles();
	soi->PerFrameInit();
	soi->BindPrograms();

	// render geometry
	for (auto& mesh : scene->rm->meshes) {
		mesh.second.DrawHW();
	}

	soi->PerFrameDisable();
	cgi->DisableProfiles();
}