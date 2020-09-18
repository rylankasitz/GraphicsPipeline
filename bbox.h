#pragma once

#include "vector.h"

class BBox
{
public:
	Vector Min, Max;
	float Size;

	BBox();
	
	void SetBBox(Vector min, Vector max);

private:
};