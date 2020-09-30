#pragma once

#include "vector.h"

class BBox
{
public:
	Vector Corners[2];
	float Size;

	BBox(Vector point);
	
	void AddPoint(Vector point);

	bool ClipWithFrame(int w, int h);

private:
};