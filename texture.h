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
	Texture(char* _tname);

	bool FindPixColor(float x, float y, Vector& color);

private:

};