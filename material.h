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
	float pfong;
	Texture texture;

	Material() {}
	Material(unsigned int _color);
	Material(unsigned int _color, Texture _texture);

	void SetPix(float x, float y);

private:
};