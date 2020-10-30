#pragma once

class FrameBuffer;
class Vector;

class Texture
{
public:
	static Texture NONE;

	char* tname;
	bool none;
	FrameBuffer* fb;

	Texture();
	Texture(int w, int h);
	Texture(char* _tname);

	int GetWidth();
	int GetHeight();

	bool FindPixColor(float x, float y, Vector& color);
	bool FindPixColorByPixel(float x, float y, Vector& color);

private:

};