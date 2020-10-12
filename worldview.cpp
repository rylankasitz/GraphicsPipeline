#include "stdafx.h"
#include "worldview.h"
#include "tmesh.h"
#include "ppc.h"
#include "light.h"
#include "scene.h"

WorldView::WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label) {

	fb = new FrameBuffer(x, y, w, h, id);
	fb->label(label);
	fb->SetBGR(0xFF0000FF);

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

void WorldView::Render(Scene* scene) {

	if (renderMode == RenderMode::Full) {
		fb->SetBGR(0xFFFFFFFF);		
	}
	fb->ClearZB();

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

	fb->redraw();
}