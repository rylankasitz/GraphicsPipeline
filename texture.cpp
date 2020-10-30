#include "stdafx.h"
#include "texture.h"
#include "framebuffer.h"
#include "vector.h"

Texture Texture::NONE = Texture();

Texture::Texture() {

	none = true;
	tname = "";
	fb = new FrameBuffer(0, 0, 0, 0, 0);
}

Texture::Texture(int w, int h) {

	none = false;
	tname = "";
	fb = new FrameBuffer(0, 0, w, h, 0);
}

Texture::Texture(char* _tname) {

	tname = _tname;
	none = false;

	fb = new FrameBuffer(0, 0, 0, 0, 0);
	fb->LoadTiff(tname);
}

int Texture::GetWidth()
{

	return fb->w;
}

int Texture::GetHeight()
{

	return fb->h;
}

bool Texture::FindPixColor(float x, float y, Vector& color) {

	if (none)
		return false;

	Vector torigin(floor(x), floor(y), 0);
	float px = ((x - torigin[0]) * (float)fb->w);
	float py = ((y - torigin[1]) * (float)fb->h);
	int u = (int)px;
	int v = (int)py;

	float tx = px - (float)u;
	float ty = py - (float)v;

	Vector c[4];
	for (int i = 0; i < 4; i++) {
		int u0 = u + (float)i * 0.5f;
		int v0 = v + (i % 2);

		if (u0 < 0 || u0 > fb->w - 1 || v0 < 0 || v0 > fb->h - 1)
			return false;

		c[i] = Vector::ZERO;
		c[i].SetFromColor(fb->pix[(fb->h - 1 - v0) * fb->w + u0]);
	}

	color = Vector(tx, ty, 0).BIntrp(c[0], c[1], c[2], c[3]);
	return true;
}

bool Texture::FindPixColorByPixel(float x, float y, Vector& color) {

	int u = (int)x;
	int v = (int)y;

	if (u < 0 || u > fb->w - 1 || v < 0 || v > fb->h - 1)
		return false;

	float tx = x - (float)u;
	float ty = y - (float)v;

	Vector c[4];
	for (int i = 0; i < 4; i++) {
		int u0 = u + (float)i * 0.5f;
		int v0 = v + (i % 2);

		if (u0 < 0 || u0 > fb->w - 1 || v0 < 0 || v0 > fb->h - 1)
		{
			u0 = u;
			v0 = v;
		}

		c[i] = Vector::ZERO;
		c[i].SetFromColor(fb->pix[(fb->h - 1 - v0) * fb->w + u0]);
	}

	color = Vector(tx, ty, 0).BIntrp(c[0], c[1], c[2], c[3]);
	return true;
}