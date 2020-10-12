#include "stdafx.h"
#include "projector.h"
#include "worldview.h"
#include "vector.h"
#include "ppc.h"
#include "scene.h"

Projector::Projector(char* tex, Vector position, Vector lookatpoint) {

	FrameBuffer* tfb = new FrameBuffer(0, 0, 0, 0, 0);
	tfb->LoadTiff(tex);

	wv = new WorldView(0, 0, 0, tfb->w, tfb->h, 30.0f, "projector");
	wv->fb->LoadTiff(tex);	
	wv->ppc->SetPose(position, lookatpoint, Vector::YAXIS);
	wv->renderMode = RenderMode::ShadowMap;

	delete tfb;
}

void Projector::RenderFB(Scene* scene) {

	wv->Render(scene);
}

bool Projector::GetProjectorColor(Vector point, Vector& color) {

	Vector pPoint;
	if (!wv->ppc->Project(point, pPoint))
		return false;

	if (!wv->fb->Closer(pPoint[0], pPoint[1], pPoint[2], .05f))
		return false;

	unsigned int colorV;
	if (!wv->fb->Get((int)pPoint[0], (int)pPoint[1], colorV))
		return false;

	color.SetFromColor(colorV);
	return true;
}