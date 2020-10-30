#pragma once

#include "vector.h"
#include "texture.h"

class Vector;
class FrameBuffer;

class Material
{
public:
	static Material Default;

	Vector color, currColor; 
	bool reflection;
	float pfong;
	Texture texture;

	Material() {}
	Material(unsigned int _color, bool refl);
	Material(unsigned int _color, Texture _texture);

	void SetPix(float x, float y);

private:
};