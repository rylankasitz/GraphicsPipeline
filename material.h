#pragma once

#include "vector.h"

class FrameBuffer;

class Material
{
public:
	static Material Default;

	Vector Color; 
	Vector CurrPixColor;
	float Pfong;

	Material(unsigned int color, float pfong);
	~Material();
	
	void LoadTexture(char* tex, bool tiled);
	void SetTexturePix(float x, float y);

private:
	FrameBuffer* fb;
	bool tiled;
};