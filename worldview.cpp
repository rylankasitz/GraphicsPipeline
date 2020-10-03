#include "stdafx.h"
#include "worldview.h"
#include "tmesh.h"
#include "ppc.h"
#include "pointlight.h"

WorldView::WorldView(unsigned int id, int x, int y, int w, int h, float hfov, const char* label) {

	fb = new FrameBuffer(x, y, w, h, id);
	fb->label(label);
	fb->SetBGR(0xFF0000FF);

	ppc = new PPC(hfov, fb->w, fb->h);
}

WorldView::~WorldView() {
	delete fb;
	delete ppc;
}

void WorldView::Show() {
	fb->show();
	fb->redraw();
}

void WorldView::Render(TMesh* meshes, int meshCount, WorldView** wvs, int wvCount, PointLight* pl) {
	fb->SetBGR(0xFFFFFFFF);
	fb->ClearZB();

	for (int i = 0; i < wvCount; i++) {
		wvs[i]->ppc->Visualize(this, 20.0f);
	}

	for (int i = 0; i < meshCount; i++) {
		meshes[i].DrawMesh(this, pl);
	}

	fb->redraw();
}