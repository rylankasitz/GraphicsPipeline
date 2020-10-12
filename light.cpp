#include "stdafx.h"
#include "light.h"
#include "material.h"
#include "worldview.h"
#include "tmesh.h"
#include "scene.h"

#include <algorithm>  

using namespace std;

Light::Light(float _ambient, float _intensity, Scene* _scene)
{
	center = Vector(100, 100, 100);
	intensity = _intensity;
	ambience = _ambient;
	scene = _scene;

	wvShadowMap = new WorldView(0, 0, 0, 2000, 2000, 120.0f, "Shadowmap");
	wvShadowMap->ppc->SetPose(center, Vector::ZERO, Vector::YAXIS);
	wvShadowMap->renderMode = RenderMode::ShadowMap;
}

void Light::Render(WorldView* wv) {

	wv->fb->Draw3DPoint(center, wv->ppc, CR_YELLOW, 10);
}

Vector Light::GetColor(Vector lightv, Vector normalv, Vector vd, Material m) {

	Vector lv = lightv * -1.0f;
	float kd = lightv * normalv; kd = (kd < 0.0f) ? 0.0f : kd;
	float ks = pow(std::max(lv.Reflect(normalv) * vd, 0.0f), m.pfong) * intensity;

	Vector color = m.currColor * (ambience + (1.0f - ambience) * kd + ks);
	color.Clamp(Vector::ZERO, Vector::ONES);
	return color;
}

void Light::RenderShadowMap() {

	wvShadowMap->Render(scene);
}

bool Light::InShadow(Vector point) {

	Vector pPoint;
	if (!wvShadowMap->ppc->Project(point, pPoint))
		return false;

	return !wvShadowMap->fb->Closer(pPoint[0], pPoint[1], pPoint[2], 0.5f);
}

#pragma region Transformations

void Light::SetPosition(Vector position) {

	center = position;
	wvShadowMap->ppc->SetPose(center, Vector::ZERO, Vector::YAXIS);
	RenderShadowMap();
}

void Light::Rotate(Vector origin, Vector direction, float angle) {

	center.RotatePoint(origin, direction, angle);
	wvShadowMap->ppc->SetPose(center, Vector::ZERO, Vector::YAXIS);
	RenderShadowMap();
}

#pragma endregion

