#include "stdafx.h"
#include "projector.h"
#include "worldview.h"
#include "ppc.h"
#include "scene.h"

Projector::Projector(char* tex, Scene* _scene) {

	FrameBuffer* tfb = new FrameBuffer(0, 0, 0, 0, 0);
	tfb->LoadTiff(tex);

	center = Vector(100, 100, 100);
	lookatPoint = Vector::ZERO;
	scene = _scene;

	wv = new WorldView(0, 0, 0, tfb->w, tfb->h, 25.0f, "projector", NULL);
	wv->fb->LoadTiff(tex);	
	wv->ppc->SetPose(center, lookatPoint, Vector::YAXIS);
	wv->renderMode = RenderMode::ShadowMap;

	delete tfb;
}

void Projector::RenderFB() {

	wv->Render(scene);
}

void Projector::Render(WorldView* wv) {

	wv->fb->Draw3DPoint(center, wv->ppc, CR_YELLOW, 10);
}

bool Projector::GetProjectorColor(Vector point, Vector& color) {

	Vector pPoint;
	if (!wv->ppc->Project(point, pPoint))
		return false;

	if (!wv->fb->Closer(pPoint[0], pPoint[1], pPoint[2], .5f))
		return false;

	unsigned int colorV;
	if (!wv->fb->Get((int)pPoint[0], (int)pPoint[1], colorV))
		return false;

	return !color.SetFromColorAlfa(colorV);
}

#pragma region Transformations

void Projector::SetPostion(Vector position, Vector lookatpoint) {

	center = position;
	lookatPoint = lookatpoint;
	wv->ppc->SetPose(center, lookatPoint, Vector::YAXIS);
	RenderFB();
}

void Projector::Rotate(Vector origin, Vector direction, float angle) {

	center.RotatePoint(origin, direction, angle);
	wv->ppc->SetPose(center, lookatPoint, Vector::YAXIS);
	RenderFB();
}

#pragma endregion
