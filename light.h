#pragma once

#include "vector.h"

class Material;

class Light
{
public:
	Vector Center;
	float Ambient;
	float Spread;
	float Intensity;

	Light();
	Light(Vector center, float ambient, float intensity);

	Vector GetColor(Vector lightv, Vector normalv, Vector vd, Material m);

private:
};