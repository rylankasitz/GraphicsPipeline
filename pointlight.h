#pragma once

#include "vector.h"

class PointLight
{
public:
	Vector Center;
	float Ambient;
	float Spread;
	float Intensity;

	PointLight();
	PointLight(Vector center, float ambient, float spread, float intensity);

	Vector Light(Vector c, Vector lightv, Vector normalv, Vector vd);

private:
};