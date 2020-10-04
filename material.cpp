#include "stdafx.h"

#include "material.h"
#include "framebuffer.h"

#include "math.h"

Material Material::Default = Material(0xFF0000FF, 32);

Material::~Material() {
	
}

Material::Material(unsigned int color, float pfong) {

	Color = Vector::ZERO;
	Color.SetFromColor(color);
	CurrPixColor = Color;
	Pfong = pfong;

	fb = new FrameBuffer(0, 0, 0, 0, 0);

	tiled = true;
}

void Material::LoadTexture(char* tex, bool _tiled) {

	fb->LoadTiff(tex);
	tiled = _tiled;
}

void Material::SetTexturePix(float x, float y) {

	if (!tiled && (x < 0.0f || x > 1.0f || y < 0.0f || y > 0.0f))
		return;

	Vector torigin(floor(x), floor(y), 0);
	float px = ((x - torigin[0]) * (float)fb->w);
	float py = ((y - torigin[1]) * (float)fb->h);
	int u = (int) px;
	int v = (int) py;

	float tx = px - (float)u;
	float ty = py - (float)v;

	Vector c[4];
	for (int i = 0; i < 4; i++) {
		int u0 = u + (float)i * 0.5f;
		int v0 = v + (i % 2);

		if (u0 < 0 || u0 > fb->w - 1 || v0 < 0 || v0 > fb->h - 1)
			return;

		c[i] = Vector::ZERO;
		c[i].SetFromColor(fb->pix[(fb->h - 1 - v0) * fb->w + u0]);
	}

	CurrPixColor = Vector(tx, ty, 0).BIntrp(c[0], c[1], c[2], c[3]);
}