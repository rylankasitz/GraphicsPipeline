#include "stdafx.h"
#include "light.h"
#include "material.h"

#include <algorithm>  

Light::Light(Vector center, float ambient, float intensity)
{
	Center = center;
	Intensity = intensity;
	Ambient = ambient;
}

Vector Light::GetColor(Vector lightv, Vector normalv, Vector vd, Material m) {

	Vector lv = lightv * -1.0f;
	float kd = lightv * normalv; kd = (kd < 0.0f) ? 0.0f : kd;
	float ks = pow(std::max(lv.Reflect(normalv) * vd, 0.0f), m.Pfong) * Intensity;
	return m.CurrPixColor * (Ambient + (1.0f - Ambient) * kd + ks);
}