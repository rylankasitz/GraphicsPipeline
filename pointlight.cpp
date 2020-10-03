#include "stdafx.h"
#include "pointlight.h"

#include <algorithm>  

PointLight::PointLight(Vector center, float ambient, float spread, float intensity)
{
	Center = center;
	Ambient = ambient;
	Spread = spread;
	Intensity = intensity;
}

Vector PointLight::Light(Vector c, Vector lightv, Vector normalv, Vector vd) {

	Vector lv = lightv * -1.0f;
	float kd = lightv * normalv; kd = (kd < 0.0f) ? 0.0f : kd;
	float ks = pow(std::max(lv.Reflect(normalv) * vd, 0.0f), Spread) * Intensity;
	return c * (this->Ambient + (1.0f - this->Ambient) * kd + ks);
}